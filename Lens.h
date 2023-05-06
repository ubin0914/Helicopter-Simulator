#include <iostream>
#include <GL/glut.h>

#include "PublicValue.h"

using namespace std;

class Lens {
public:
	bool dirFixed;
	Coordinate locate_r;
	Coordinate axisX_r;
	Coordinate axisY_r;
	Coordinate axisZ_r;
	int sightMode;
	int focusMode;
	Coordinate locate_h;	//以helicopter座標系為基底，僅follow時(sightMode!=3)用到
	//sightMode=3時會直接控制locate_r，否則locate_r仰賴由locate_h同步
	int projectMode;		//0: ortho投影	1: perspective投影
	float sectionWidth;
	float sectionHeight;
	//以下僅perspective投影用得到
	float magnification;
	float nearDist;
	float farDist;

	Lens(bool dirFixed, Coordinate locate_r, Coordinate axisX_r, Coordinate axisY_r, Coordinate axisZ_r,
		int sightMode = 2, int focusMode = 0, int projectMode = 1, float sectionWidth = 30, float sectionHeight = 30, float magnification = 1.5, float nearDist = 10, float farDist = 50)
	{
		this->locate_r = locate_r;
		this->axisX_r = axisX_r;
		this->axisY_r = axisY_r;
		this->axisZ_r = axisZ_r;
		this->projectMode = projectMode;
		this->sectionWidth = sectionWidth;
		this->sectionHeight = sectionHeight;
		this->magnification = magnification;
		this->nearDist = nearDist;
		this->farDist = farDist;
	}
	void evolve(int extent = 1) {
		//固定鏡頭
		if (dirFixed) {
			if (sightMode != 3) locate_r = helicopter.origin_r + locate_h;
		}
		//可調鏡頭
		else if (sightMode == 1) {
			locate_r = helicopter.origin_r + locate_h;
		}
		else if (sightMode == 2) {
			locate_r = helicopter.origin_r + locate_h;
		}
		else if (sightMode == 3) {
			float cosV, sinV;
			if (extent == 1) cosV = cos_1d, sinV = sin_1d;
			// 旋轉/平移 操作
			if (monitorOperateMode) {
				if (spKeyPressing[UPKEY_INT]) rotateDualAxis(axisY_r, axisZ_r, cosV, sinV);
				if (spKeyPressing[DOWNKEY_INT]) rotateDualAxis(axisZ_r, axisY_r, cosV, sinV);
				if (spKeyPressing[LEFTKEY_INT]) rotateDualAxis(axisZ_r, axisX_r, cosV, sinV);
				if (spKeyPressing[RIGHTKEY_INT]) rotateDualAxis(axisX_r, axisZ_r, cosV, sinV);
				if (spKeyPressing[R_SHIFT_INT]) rotateDualAxis(axisY_r, axisX_r, cosV, sinV);
				if (spKeyPressing[R_CTRL_INT]) rotateDualAxis(axisX_r, axisY_r, cosV, sinV);
			}
			else {
				if (spKeyPressing[UPKEY_INT]) locate_r += eyeAxisY * extent;
				if (spKeyPressing[DOWNKEY_INT]) locate_r -= eyeAxisY * extent;
				if (spKeyPressing[LEFTKEY_INT]) locate_r -= eyeAxisX * extent;
				if (spKeyPressing[RIGHTKEY_INT]) locate_r += eyeAxisX * extent;
				if (spKeyPressing[R_SHIFT_INT]) locate_r -= eyeAxisZ * extent;
				if (spKeyPressing[R_CTRL_INT]) locate_r += eyeAxisZ * extent;
			}
		}
	}
	void setView() {
		if (dirFixed) {

		}
		else {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(locate_r.x, locate_r.y, locate_r.z,
				locate_r.x - axisZ_r.x, locate_r.y - axisZ_r.y, locate_r.z - axisZ_r.z,
				axisY_r.x, axisY_r.y, axisY_r.z);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(120 / magnification, (double)width / (double)height, nearDist, farDist);
			glMatrixMode(GL_MODELVIEW);
		}
	}
	void display() {

	}
};