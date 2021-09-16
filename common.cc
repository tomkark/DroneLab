
#ifndef _COMMON_CC
#define _COMMON_CC

#include <iostream>
#include <math.h>
#include <Converter.h>
#include <System.h>
#include <ctello.h>
#include <unistd.h>
#include <signal.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <thread>
#include <complex>

#include "opencv2/opencv.hpp"
using namespace std;
using namespace chrono;
using namespace cv;
using namespace ctello;



#define fin(i,s,n) for(auto i = s; i < n; ++i)
#define pb push_back
using ld = long double;
//used when comparing numbers (if numbers are different by less than epsilon they are probably equal)
const ld epsilon = 1e-6;
//A very accurate PI
#define PI 3.141592653589793238462643383279502884L

//Scan Rotation Angle and Scan Vertical Movement respectively
#define svm 20
#define sra 25

//A struct that represents a point in 2D 
//under x,z coordinates
//or a vector in 2D
struct p2d {
    ld x, z;
    //dot product
    ld operator*(p2d o) { return (x * o.x + z * o.z); }

    //multiply / divide by scalar
    p2d operator*(ld o) { return {x*o, z*o}; }
    p2d operator/(ld o) { return {x/o,z/o}; }

    //other operations(such as addition and subtraction)
    p2d operator+(p2d o) { return {x + o.x, z + o.z}; }
    p2d operator-() { return {-x, -z}; }
    p2d operator-(p2d o) { return {x - o.x, z - o.z}; }

    //vector operations such as length(abs), angle from x axis, and other
    ld abs() { return sqrtl(x * x + z * z); }
    ld angle() { return atan2l(z, x)*180.0/PI; }
    ld positive_angle() {
        ld a = angle();
        if(a < -epsilon) a += 360.0;
        return a;
    }
    ld dot_norm(p2d o) {
        return ((*this)*o)/(abs()*o.abs());
    }
    static p2d polar(ld theta) { return {cos(theta), sin(theta)}; }
};

//A struct that represents a point in 3D 
//under x,y,z coordinates
//or a vector in 3D
struct p3d {
    ld x, y, z;
    //conversion operator from p3d to p2d by removing y coordinate
    explicit operator p2d() { return {x, z}; }

    //multiply / divide by scalar
    p3d operator*(ld o) { return {x*o, y*o, z*o}; }
    p3d operator/(ld o) { return {x/o,y/o,z/o}; }

    //other operations(such as addition and subtraction)
    p3d operator+(p3d o) { return {x + o.x, y + o.y, z + o.z}; }
    p3d operator-() { return {-x, -y, -z}; }
    p3d operator-(p3d o) { return {x - o.x, y-o.y, z - o.z}; }

    //length of vector
    ld abs() { return sqrtl(x * x + y * y + z * z); }
    //dot product
    ld operator*(p3d o) { return (x * o.x + y * o.y + z * o.z); }
};

//some type definions
//v prefix repesents vectors
using vp2d = vector<p2d>;
using vp3d = vector<p3d>;
using vi = vector<int>;
using pld = pair<ld,ld>;
using vld = vector<ld>;
using vpld = vector<pld>;

//A struct that represents a line (y = mx+b)
struct Line {
    ld m,b;
    //return a vector in the direction of the line 
    //if line is x=3 then slope is infinity and the vector is (0,1)
    //otherwise the vector is (1,m)
    p2d vec() { if(m==INFINITY) return {0.0,1.0}; return {1.0,m}; }
};

//adding ostream operators so we can print points,pairs and vectors easily
ostream& operator<<(ostream& os, Line l) { return os << "(m=" << l.m <<",b="<<l.b<<")"; }
ostream &operator<<(ostream &os, p2d p) { return os << '{' << p.x << ',' << p.z << '}'; }
ostream &operator<<(ostream &os, p3d p) { return os << '{' << p.x << ',' << p.y << ',' << p.z << '}'; }
template<class A, class B> ostream& operator<<(ostream& os, pair<A,B> p) {
    return os << '{' << p.first << ',' << p.second << '}';
}
template<class T> ostream& operator<<(ostream& os, vector<T> v) {
    if(v.empty()) return os << "[]";
    os << '[' << v[0];
    fin(i,1,int(v.size())) os << ',' << v[i];
    return os << ']';
}

//A small helper function to convert 3x1 vector to our p3d class
p3d matToPoint(cv::Mat m) {
    ld x = ld(m.at<float>(0));
    ld y = ld(m.at<float>(1));
    ld z = ld(m.at<float>(2));
    return {x,y,z};
}
//A reliable sleep function.
void csleep(int ms) {
    this_thread::sleep_for(milliseconds(ms));
}
double time_since_start() {
    static bool init = false;
    static chrono::time_point<high_resolution_clock> start;
    if(!init) {
        start = high_resolution_clock::now();
        init = true;
    }
    auto cur = high_resolution_clock::now();
    duration<double> dur = (cur-start);
    return dur.count();
}
#endif