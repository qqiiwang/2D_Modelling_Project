#define FREEGLUT_STATIC 
#include <math.h> 
#include <iostream> 
#include <gl/freeglut.h> 
#define MAX_CHAR 128

const GLfloat Pi = acos(-1.0);

// Some global points needed to draw the fireworks
//Declare a structure to save x,y.
typedef struct { GLfloat x, y; } point;
point p_right = { 100,400 };  //right
point p_left = { 100,400 };   //left
point p_up = { 100,400 };     //up
point p_down = { 100,400 };   //down
point p_ne = { 100,400 };  //the line pointing to northeast
point p_nw = { 100,400 };  //the line pointing to northwest
point p_se = { 100,400 };  //southeast
point p_sw = { 100,400 };  //southwest

//step for fireworks 
GLfloat step = 10.0;
GLfloat pace = 0.8;

GLfloat branch_h = 100.0f;   //the height of branch 

GLfloat mouse_times = 0;	// the number of time clicked

//smiling face
GLfloat x_face = 0;		//the x position of the smiling face
GLfloat y_face = 0;		//the y position of the smiling face

//position variables for two pinballs
GLfloat bg_circle_x = 100.0f;
GLfloat bg_circle_y = 200.0f;
GLfloat bg_circle_x2 = 600.0f;
GLfloat bg_circle_y2 = 100.0f;

GLfloat bg_step_x = 3;
GLfloat bg_step_y = 2;
GLfloat bg_step_x2 = -2;
GLfloat bg_step_y2 = 2.4;

//setting text
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		lists = glGenLists(MAX_CHAR);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}

	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}

//setting font
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//draw the branches of flowers
void drawbranch(void) {
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINES);
	glVertex2f(100.0f, 0.0f);
	glVertex2f(100.0f, branch_h);
	glEnd();

	//right sub-branch
	glBegin(GL_LINES);
	glVertex2f(100.0f, branch_h/2);
	glVertex2f(100.0f + 10.0f, branch_h / 2+10.0f);
	glEnd();

	//left sub-branch
	glBegin(GL_LINES);
	glVertex2f(100.0f, branch_h / 4);
	glVertex2f(100.0f - 10.0f, branch_h / 4 + 10.0f);
	glEnd();

	//right side of the leaves
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(100.0f+25, branch_h / 2+25);
	glVertex2f(100.0f + 15.0f, branch_h / 2 + 5.0f);
	glVertex2f(100.0f + 5.0f, branch_h / 2 + 15.0f);
	glEnd();

	//left side of leaves
	glBegin(GL_TRIANGLES);
	glVertex2f(100.0f-25, branch_h / 4+25);
	glVertex2f(100.0f - 15.0f, branch_h / 4 + 5.0f);
	glVertex2f(100.0f - 5.0f, branch_h / 4 + 15.0f);
	glEnd();

}

//draw the first pinball
void draw_background1() {
	int n = 100;
	glColor3f(200.0 / 255.0, 186.0 / 255.0, 242.0 / 255);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
			glVertex2f(50 * cos(2 * Pi / n * i) , 50 * sin(2 * Pi / n * i) );
	}
	glEnd();

}

//draw the second pinball
void draw_background2() {
	int n = 100;
	glColor3f(150.0 / 255.0, 150.0 / 255.0, 250.0 / 255);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		glVertex2f(50 * cos(2 * Pi / n * i), sin(2 * Pi / n * i) * 50);
	}
	glEnd();

}

//draw a semi-ellipse as a petal
void draw_petals() {
	int n = 100;
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		if(0.5 * sin(2 * Pi / n * i)>=0)	//only half of an ellipse is needed
		glVertex2f(0.15 * cos(2 * Pi / n * i) * 50, 0.6 * sin(2 * Pi / n * i) * 50);
	}
	glEnd();
}

//draw a blooming flower with mouse interaction
void blooming_flower() {
	draw_petals();
	if (mouse_times == 5) {
		mouse_times = 0;
	}
	//when the number of clicks add one, two petals will display in two sides symmetrically
	for (int i = 1; i <= mouse_times; i++)
	{
		glPushMatrix();
		glRotatef(i* 45, 0, 0, 1);
		draw_petals();
		glRotatef(i * -45 * 2, 0, 0, 1);
		draw_petals();
		glPopMatrix();
	}
}

//draw a smiling face with keyboard interaction
void draw_face() {
	int n1 = 200;
	int n2 = 400;
	
	//draw a circle as face
	glBegin(GL_POLYGON);
	for (int i = 0; i < 200; ++i) {
		glVertex2f(0.5f * cos(2 * Pi * i / n1) * 100, 0.5f * sin(2 * Pi * i / n1) * 100);
	}
	glEnd();
	
	//draw a mouse
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 200; i < 400; ++i) {
		glVertex2f(0.3f * cos(2 * Pi * i / n2) * 100, 0.3f * sin(2 * Pi * i / n2) * 100);
	}
	glEnd();

	//draw the left eye
	glTranslatef(-15, 20, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 200; ++i) {
		glVertex2f(0.1f * cos(2 * Pi * i / n2) * 100, 0.1f * sin(2 * Pi * i / n2) * 100);
	}
	glEnd();

	//draw the right eye
	glTranslatef(30, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 200; ++i) {
		glVertex2f(0.1f * cos(2 * Pi * i / n2) * 100, 0.1f * sin(2 * Pi * i / n2) * 100);
	}
	glEnd();
	glPopMatrix();
	
}

//draw fireworks
void draw_firework() {
	//line_right
	glBegin(GL_LINES);
	glVertex2f(p_right.x, p_right.y);
	glVertex2f(p_right.x + step, p_right.y);
	glEnd();

	//line_left
	glBegin(GL_LINES);
	glVertex2f(p_left.x, p_left.y);
	glVertex2f(p_left.x - step, p_left.y);
	glEnd();

	//line_up
	glBegin(GL_LINES);
	glVertex2f(p_up.x, p_up.y);
	glVertex2f(p_up.x, p_up.y + step);
	glEnd();

	//line_down
	glBegin(GL_LINES);
	glVertex2f(p_down.x, p_down.y);
	glVertex2f(p_down.x, p_down.y - step);
	glEnd();

	//line_northeast
	glBegin(GL_LINES);
	glVertex2f(p_ne.x, p_ne.y);
	glVertex2f(p_ne.x + step, p_ne.y + step);
	glEnd();

	//line_sortheast
	glBegin(GL_LINES);
	glVertex2f(p_se.x, p_se.y);
	glVertex2f(p_se.x + step, p_se.y - step);
	glEnd();

	//line_northwest
	glBegin(GL_LINES);
	glVertex2f(p_nw.x, p_nw.y);
	glVertex2f(p_nw.x - step, p_nw.y + step);
	glEnd();

	//line_sorthwest
	glBegin(GL_LINES);
	glVertex2f(p_sw.x, p_sw.y);
	glVertex2f(p_sw.x - step, p_sw.y - step);
	glEnd();
}

//display function
void myDisplay(void) {

	glClearColor(0, 0, 0, 0);   //r,g,b,a
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(0, 0, 800, 500);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	
	// setting background
	glShadeModel(GL_SMOOTH);
	glBegin(GL_POLYGON);
	glColor3f(242.0/255.0, 186.0/255.0, 232.0/255.0);
	glVertex2f(0, 0);
	glVertex2f(800, 0);
	glColor3f(30.0 / 255.0, 123.0 / 255.0, 180.0 / 255.0);
	glVertex2f(800, 500);
	glVertex2f(0, 500);
	glEnd();
	
	//setting moving pinballs in the background
	glPushMatrix();
	glTranslatef(bg_circle_x, bg_circle_y, 0);
	draw_background1();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(bg_circle_x2, bg_circle_y2, 0);
	draw_background2();
	glPopMatrix();
	

	//draw a smiling face
	glPushMatrix();
	glTranslatef(380+x_face, 350+y_face, 0);
	glRotatef(20, 0.0f, 0.0f, 1.0f);
	glColor3f(250/255.0, 250/255.0, 0.0);
	draw_face();
	glPopMatrix();

	//draw fireworks
	glPushMatrix();
	glColor3f(1, 0, 0);
	draw_firework();
	glTranslatef(600, 0, 0);
	draw_firework();

	glColor3f(0, 1, 0);
	glTranslatef(-100, 0, 0);
	draw_firework();
	glTranslatef(-400, 0, 0);
	draw_firework();

	glColor3f(0, 1, 1);
	glTranslatef(-50, -50, 0);
	draw_firework();
	glTranslatef(500, 0, 0);
	draw_firework();
	glPopMatrix();
	

	//display text information
	glPushMatrix();
	selectFont(40, ANSI_CHARSET, "Comic sans");
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(210.0, 300.0);	  //setting text starting position
	drawString("15TH");

	glBegin(GL_QUADS);				//decorative underline
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(210, 295, 0);
	glVertex3f(210, 290, 0);
	glVertex3f(310, 290, 0);
	glVertex3f(310, 295, 0);
	glEnd();

	glRasterPos2f(210.0, 250.0);
	drawString(" ANNIVERSARY of XJTLU!");

	glBegin(GL_QUADS);              //decorative underline
	glVertex3f(210,245,0);
	glVertex3f(210, 240, 0);
	glColor3f(242.0 / 255.0, 186.0 / 255.0, 232.0 / 255.0);
	glVertex3f(630, 240, 0);
	glVertex3f(630, 245, 0);
	glEnd();
	glPopMatrix();

	//draw branches of flowers
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	drawbranch();
	glTranslated(100.0, 0, 0);
	drawbranch();
	glTranslated(400.0, 0, 0);
	drawbranch();
	glTranslated(100.0, 0, 0);
	drawbranch();
	glPopMatrix();

	//draw flower
	glPushMatrix();
	glColor3f(254/255.0, 67/255.0, 101/255.0);
	glTranslatef(100, branch_h, 0);
	blooming_flower();
	glColor3f(252/255.0, 157/255.0, 154/255.0);
	glTranslatef(100, 0, 0);
	blooming_flower();
	glTranslatef(400, 0, 0);
	blooming_flower();
	glColor3f(254/255.0, 67/255.0, 101/255.0);
	glTranslatef(100, 0, 0);
	blooming_flower();
	glPopMatrix();



	//draw the simplified shape of the central building by a cube as the main body and a torus by the base
	//Because it is drawn by a 3d object, so here set a new lookat point and projection.
	glViewport(200, -100, 400, 400);  // define a new viewport 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluLookAt(4, 5, -1, 0, 0, 0, 0, 0, 1);    

	//draw a decorative wire cube with dotted line
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0x3F07);   // draw dotted line
	glutWireCube(3);
	glPopMatrix();
	glDisable(GL_LINE_STIPPLE);

	
	//draw a torus as the base 
	glPushMatrix();
	glColor3f(30.0 / 255.0, 123.0 / 255.0, 160.0 / 255.0);
	glTranslatef(0, 0, -1);
	glutSolidTorus(0.3, 1.4, 12, 12);
	glPopMatrix();

	//draw main sreucture of the central building
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCube(2);


	//structural decoration of the Central Building
	glColor3f(12.0 / 255.0, 123.0 / 255.0, 179.0 / 255.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.8, 1, 1);
	glVertex3f(0.8, 1, 0.7);
	glVertex3f(0.6, 1, 0.7);
	glVertex3f(0.5, 1, 1);
	glEnd();

	//draw structure line
	glBegin(GL_LINE_STRIP);
	glVertex3f(1, 1, -0.5);
	glVertex3f(0, 1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();
	

	glBegin(GL_LINE_STRIP);
	glVertex3f(1, 1, 0.2);
	glVertex3f(0.2, 1, 0.2);
	glVertex3f(-0.3, 1, 0.7);
	glVertex3f(-0.7, 1, 0.7);
	glVertex3f(-1, 1, 0.1);
	glEnd();

	//front side
	glBegin(GL_LINE_STRIP);
	glVertex3f(1, 1, -0.5);
	glVertex3f(1, 0, 0);
	glVertex3f(1, -1, 0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(1, 1, 0.2);
	glVertex3f(1, 0.5, 0.2);
	glVertex3f(1, 0.2, 1);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(1, -0.1,1);
	glVertex3f(1, -0.2, 0.3);
	glVertex3f(1, -1, 0.3);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

//add a callback function
void when_in_loop() {
	glutPostRedisplay();
}

void onTimer(int value) {
	//setting fireworks position 
	p_right.x +=1;  //every time interval, the point controling right line is added a pace   
	p_left.x -= 1;
	p_up.y += 1;
	p_down.y -=1;

	p_ne.x += pace;
	p_ne.y += pace;
	p_nw.x -= pace;
	p_nw.y += pace;
	p_se.x += pace;
	p_se.y -= pace;
	p_sw.x -= pace;
	p_sw.y -= pace;

	if (p_right.x > 150) {
		p_right.x = 100;
		p_left.x = 100;
		p_up.y = 400;
		p_down.y = 400;

		p_ne.x = 100;
		p_ne.y = 400;

		p_se.x = 100;
		p_se.y = 400;

		p_nw.x = 100;
		p_nw.y = 400;

		p_sw.x = 100;
		p_sw.y = 400;
	}

	
	//if the pinballs moving exceed the boundary
	if (bg_circle_x > 750 || bg_circle_x < 50)
		bg_step_x=-bg_step_x;

	if (bg_circle_y > 450 || bg_circle_y < 50)
		bg_step_y=-bg_step_y;

	if (bg_circle_x2 > 750 || bg_circle_x2 < 50)
		bg_step_x2 = -bg_step_x2;

	if (bg_circle_y2 > 450 || bg_circle_y2 < 50)
		bg_step_y2 = -bg_step_y2;
	
	//if two balls collide
	if (sqrt(pow((bg_circle_x - bg_circle_x2),2) + pow((bg_circle_y - bg_circle_y2), 2)) < 100) {
		bg_step_x = -bg_step_x;
		bg_step_x2 = -bg_step_x2;
		bg_step_y = -bg_step_y;
		bg_step_y2 = -bg_step_y2;
	}

	bg_circle_x += bg_step_x;
	bg_circle_y += bg_step_y;
	bg_circle_x2 += bg_step_x2;
	bg_circle_y2 += bg_step_y2;

	glutTimerFunc(40, onTimer, 1);
}

void myReshape(GLsizei w, GLsizei h) {
	glViewport(200, 100, 400, 400);  // define viewport 

	glMatrixMode(GL_PROJECTION);  // projection transformation 
	glLoadIdentity();  // clear the matrix 
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluLookAt(5, 4, -1, 0, 0, 0, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);  // back to modelview matrix 
	glLoadIdentity();  // clear the matrix 
}

//keyboard interactio
void keyboard_input(unsigned char key, int x, int y) {
	//Type "w" to move up
	if (key == 'w' || key == 'W')
		y_face += 5.0f;

	//Type "s" to move down
	else if (key == 's' || key == 'S')
		y_face -= 5.0f;

	//Type "d" to move right
	else if (key == 'd' || key == 'D')
		x_face += 5.0f;

	//Type "a" to move left
	else if (key == 'a' || key == 'A')
		x_face -= 5.0f;

	//If the smiling face touches to the windoew boundary
	if (x_face < -700 || x_face > 60)
		x_face = 0;
	if (y_face < -280 || y_face > 180)
		y_face = 0;
}

//mouse interaction
void mouse_input(int button, int state, int x, int y) {
	//Press left button to control the growing process of flowers
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		branch_h += 20;
		mouse_times++;
	}
	
	//If the height of flower exceeds the limitaion
	if (branch_h > 180)
		branch_h = 100;
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 500);
	glutCreateWindow("15TH ANNIVERSARY of XJTLU");

	
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(when_in_loop);
	glutTimerFunc(40, onTimer, 1);
	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouse_input);
	glutMainLoop();
}