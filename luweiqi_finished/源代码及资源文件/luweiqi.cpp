#include <GL\glut.h>
#include <iostream> 
#include <string>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
//定义圆周率
#define PI 3.14159265358979323846

using namespace std;

//读文件
int num_texture_file;

string *texture_file_name;

int num_texture_quality;
//决定显示模式
int flag = 3;

struct texture_quality
{
	GLfloat ambient[4];//环境光
	GLfloat diffuse[4];//漫反射
	GLfloat specular[4];//镜面反射
	GLfloat emission[4];//辐射
	GLfloat shiness;//光斑
	int texture_index;
};

texture_quality * texture_quality_list;
//顶点个数
int num_vertexs;
//顶点数组
struct vertex
{
	GLfloat xyz[4];
};
vertex*vertex_list;

int num_map;
//贴图
struct map
{
	GLfloat xy[2];
};
map*map_list;

int num_normal;
//法线
struct normal
{
	GLfloat xyz[3];
};
normal*normal_list;

int num_sub_model;

GLfloat scale[3];

struct triangle
{
	int abc[9];
};

struct sub_model
{
	int triangle_num;
	triangle * triangle_list;
	int quality_index;
};
sub_model* sub_model_list;

//停止声明结构体

// 存储多个纹理
GLuint*  texture;

/*透视观察适用初始化参数，实际上图片素材并不适合透视查看
//平移
GLdouble translateX = -1.6, translateY = -3.3, translateZ = 2.1;
//旋转
GLdouble rotateAngle = 0.4, rotateX = -1, rotateY = 1, rotateZ = 0;
//缩放
GLdouble scaleX = 1, scaleY = 1, scaleZ = 1;
//投影
GLdouble Theta = 3.34, aspect = 1, dnear = 8.5, dfar = 0.06;
//三维观察
GLdouble x_0 = -0.3, y_0 = -0.2, z_0 = 0, xref = -0.35, yref = -0.35, zref = 0.1, V_X = 0.1, V_Y = 0.05, V_Z = 0.75;
*/

//平移
GLdouble translateX = 0, translateY = 0, translateZ = 0;
//旋转
GLdouble rotateAngle = 0, rotateX = -1, rotateY = 1, rotateZ = 0;
//缩放
GLdouble scaleX = 0.8, scaleY = 0.8, scaleZ = 0.8;
//投影
GLdouble Theta = 0, aspect = 0, dnear = 0, dfar = 0;
//三维观察
GLdouble x_0 = 0.1, y_0 = 0.2, z_0 = 0, xref = -0.1, yref = -0.4, zref = 0, V_X = 0, V_Y = 0, V_Z = 1.0;

//控制指针
GLdouble *indexA = NULL, *indexB = NULL, *indexC = NULL, *indexD = NULL;

void readFile(void)
{
	string s;

	string file_name = "luweiqi.txt";

	ifstream fin(file_name);
	if (!fin) { cout << "Error opening " << file_name << " for input" << endl; system("pause"); exit(-1); }
	//读纹理文件名
	fin >> s;
	num_texture_file = atoi(s.c_str());
	cout <<"file_Number="<< num_texture_file << endl;
	texture_file_name = new string[num_texture_file];
	texture = new GLuint[num_texture_file];
	for (int i = 0; i < num_texture_file; i++)
	{
		fin >> texture_file_name[i];
	}

	//读纹理文件的光照属性
	fin >> s;
	num_texture_quality = atoi(s.c_str());
	cout << "quality_Number=" << num_texture_quality << endl;
	texture_quality_list = new texture_quality[num_texture_quality];
	for (int i = 0; i < num_texture_quality; i++)
	{
		for (int j = 0; j < 4; j++) {
			fin >> s;
			texture_quality_list[i].ambient[j] = (GLfloat)atof(s.c_str());
		}
		for (int j = 0; j < 4; j++) {
			fin >> s;
			texture_quality_list[i].diffuse[j] = (GLfloat)atof(s.c_str());
		}
		for (int j = 0; j < 4; j++) {
			fin >> s;
			texture_quality_list[i].specular[j] = (GLfloat)atof(s.c_str());;
		}
		for (int j = 0; j < 4; j++) {
			fin >> s;
			texture_quality_list[i].emission[j] = (GLfloat)atof(s.c_str());
		}
		fin >> s;
		texture_quality_list[i].shiness = (GLfloat)atof(s.c_str());
		fin >> s;
		texture_quality_list[i].texture_index = atoi(s.c_str()) - 1;
	}

	//读顶点
	fin >> s;
	num_vertexs = atoi(s.c_str());
	cout << "vertex_Number=" << num_vertexs << endl;
	vertex_list = new vertex[num_vertexs];
	for (int i = 0; i < num_vertexs; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fin >> s;
			vertex_list[i].xyz[j] = (GLfloat)atof(s.c_str());
		}
		vertex_list[i].xyz[3] = 1.0;
	}

	//读贴图
	fin >> s;
	num_map = atoi(s.c_str());
	cout << "map_Number=" << num_map << endl;
	map_list = new map[num_map];
	for (int i = 0; i < num_map; i++)
	{
		fin >> s;
		map_list[i].xy[0] = (GLfloat)atof(s.c_str());
		fin >> s;
		map_list[i].xy[1] = (GLfloat)atof(s.c_str());
	}

	//读法线
	fin >> s;
	num_normal = atoi(s.c_str());
	cout << "normal_Number=" << num_normal << endl;
	normal_list = new normal[num_normal];
	for (int i = 0; i < num_normal; i++)
	{
		fin >> s;
		normal_list[i].xyz[0] = (GLfloat)atof(s.c_str());
		fin >> s;
		normal_list[i].xyz[1] = (GLfloat)atof(s.c_str());
		fin >> s;
		normal_list[i].xyz[2] = (GLfloat)atof(s.c_str());
	}

	//子模型数量
	fin >> s;
	num_sub_model = atoi(s.c_str());
	cout << "submodel_Number=" << num_sub_model << endl;
	sub_model_list = new sub_model[num_sub_model];

	//缩放
	for (int i = 0; i < 3; i++)
	{
		fin >> s;
		scale[i] = (GLfloat)atof(s.c_str());
	}

	//子模型
	for (int i = 0; i < num_sub_model; i++)
	{
		fin >> s;
		sub_model_list[i].triangle_num = atoi(s.c_str());
		cout << "tri_Number=" << sub_model_list[i].triangle_num << endl;
		sub_model_list[i].triangle_list = new triangle[sub_model_list[i].triangle_num];

		fin >> s;
		sub_model_list[i].quality_index = atoi(s.c_str()) - 1;

		for (int j = 0; j < sub_model_list[i].triangle_num; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				fin >> s;
				sub_model_list[i].triangle_list[j].abc[k] = atoi(s.c_str());
			}
		}
	}
	fin.close();
}
//加载纹理
int LoadGLTextures(const char *FileName, GLuint *ttexture)
{
	glGenTextures(1, ttexture);
	char *Filename = new char[100];
	strcpy_s(Filename, 100, FileName);
	BITMAPINFOHEADER header;
	BYTE *texture;
	int width, height, m;
	unsigned char *image;
	ifstream File(Filename, ios::binary);
	//读取BMP信息头，跳过14字节文件头
	if (File) {
		File.seekg(14, SEEK_SET);
		File.read( (char *)&header, sizeof(BITMAPINFOHEADER));
	}
	else { cout << "Error opening " << Filename << " for input" << endl; system("pause"); exit(-1); }
	//读取长宽
	width = abs(header.biWidth);
	height = abs(header.biHeight);
	//判断位图类型并标记
	int Type;
	if (header.biBitCount == 32) Type = 4;
	else Type = 3;
	//为image分配像素空间
	image = new unsigned char [width*height*Type];

	File.read( (char *)image, width*height*Type*sizeof(unsigned char));
	
	texture = new BYTE [width*height*Type];
	//对texture赋值
	if (Type == 4)
	{
		for (m = 0; m < width*height; m++)
		{
			texture[4 * m] = image[4 * m + 2];
			texture[4 * m + 1] = image[4 * m + 1];
			texture[4 * m + 2] = image[4 * m];
			texture[4 * m + 3] = image[4 * m + 3];
		}
	}
	else
	{
		for (m = 0; m < width*height * 3; m++)
		{
			texture[m] = image[m];
		}
	}
	//指定纹理属性
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//对齐
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//绑定
	glBindTexture(GL_TEXTURE_2D, *ttexture);
	//环绕与过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(Type==4) glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	else glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture);
	glBindTexture(GL_TEXTURE_2D, *ttexture);
	//释放内存
	delete texture;
	delete image;
	delete Filename;
	//关闭文件
	File.close();
	//成功加载
	return TRUE;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//在display中设置投影和观察视角，这样才可以在每次修改参数后显示
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//透视投影函数
	gluPerspective(Theta * 5 * PI, aspect, dnear, dfar);
	//三维观察函数
	gluLookAt(x_0, y_0, z_0,  xref, yref, zref,  V_X, V_Y, V_Z);
	//平移
	glTranslatef((GLdouble)translateX, (GLdouble)translateY, (GLdouble)translateZ);
	//旋转
	glRotatef((GLdouble)rotateAngle * 5 * PI, (GLdouble)rotateX, (GLdouble)rotateY, (GLdouble)rotateZ);
	//放缩
	glScalef((GLdouble)scaleX, (GLdouble)scaleY, (GLdouble)scaleZ);

	for (size_t i = 0; i < num_sub_model; i++)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, texture_quality_list[sub_model_list[i].quality_index].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, texture_quality_list[sub_model_list[i].quality_index].diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, texture_quality_list[sub_model_list[i].quality_index].specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, texture_quality_list[sub_model_list[i].quality_index].emission);
		glMaterialf(GL_FRONT, GL_SHININESS, texture_quality_list[sub_model_list[i].quality_index].shiness);

		//载入并显示纹理
		LoadGLTextures(texture_file_name[i].c_str(), &texture[i]);

		for (size_t j = 0; j < sub_model_list[i].triangle_num; j++)
		{
			//选择显示模式
			if ( flag == 1) glBegin(GL_POINTS);
			else if ( flag == 2) glBegin(GL_LINE_LOOP);
			else glBegin(GL_TRIANGLES);

			glNormal3fv(normal_list[sub_model_list[i].triangle_list[j].abc[2] - 1].xyz);
			glTexCoord2f(map_list[sub_model_list[i].triangle_list[j].abc[1] - 1].xy[0], map_list[sub_model_list[i].triangle_list[j].abc[1] - 1].xy[1]);
			glVertex3fv(vertex_list[sub_model_list[i].triangle_list[j].abc[0] - 1].xyz);

			glNormal3fv(normal_list[sub_model_list[i].triangle_list[j].abc[5] - 1].xyz);
			glTexCoord2f(map_list[sub_model_list[i].triangle_list[j].abc[4] - 1].xy[0], map_list[sub_model_list[i].triangle_list[j].abc[4] - 1].xy[1]);
			glVertex3fv(vertex_list[sub_model_list[i].triangle_list[j].abc[3] - 1].xyz);

			glNormal3fv(normal_list[sub_model_list[i].triangle_list[j].abc[8] - 1].xyz);
			glTexCoord2f(map_list[sub_model_list[i].triangle_list[j].abc[7] - 1].xy[0], map_list[sub_model_list[i].triangle_list[j].abc[7] - 1].xy[1]);
			glVertex3fv(vertex_list[sub_model_list[i].triangle_list[j].abc[6] - 1].xyz);

			glEnd();

		}

	}

	//画出坐标轴
	glBegin(GL_LINES);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glEnd();

	//输出缓冲区
	glFlush();

}
//键盘控制选项
void keyBoardFunc(unsigned char key, int x, int y)
{
	//选择点、线、面模式
	if (key == '1') flag = 1;
	if (key == '2') flag = 2;
	if (key == '3') flag = 3;
	//改变参数
	if (key == 'a')
		if (indexA) *indexA += 0.05;
	if (key == 'z')
		if (indexA) *indexA -= 0.05;
	if (key == 's')
		if (indexB) *indexB += 0.05;
	if (key == 'x')
		if (indexB) *indexB -= 0.05;
	if (key == 'd')
		if (indexC) *indexC += 0.05;
	if (key == 'c')
		if (indexC) *indexC -= 0.05;
	if (key == 'f')
		if (indexD) *indexD += 0.05;
	if (key == 'v')
		if (indexD) *indexD -= 0.05;

	if (key == 'g'){
		if (indexA) *indexA += 0.05;
		if (indexB) *indexB += 0.05;
		if (indexC) *indexC += 0.05;
		if (indexD) *indexD += 0.05;
	}
		
	if (key == 'b') {
		if (indexA) *indexA -= 0.05;
		if (indexB) *indexB -= 0.05;
		if (indexC) *indexC -= 0.05;
		if (indexD) *indexD -= 0.05;
	}
		
	//清空命令行屏幕
	system("cls");

	//输出参数方便调整
	printf("平移参数:\t%.2lf\t%.2lf\t%.2lf\n", translateX, translateY, translateZ);
	printf("放缩参数:\t%.2lf\t%.2lf\t%.2lf\n", scaleX, scaleY, scaleZ);
	printf("旋转参数:\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", rotateAngle * 5 * PI, rotateX, rotateY, rotateZ);
	printf("透视参数:\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", Theta * 5 * PI, aspect, dnear, dfar);
	printf("观察点:  \t%.2lf\t%.2lf\t%.2lf\n", x_0, y_0, z_0);
	printf("参考点:  \t%.2lf\t%.2lf\t%.2lf\n", xref, yref, zref);
	printf("向上向量:\t%.2lf\t%.2lf\t%.2lf\n\n", V_X, V_Y, V_Z);

	display();

}

//根据菜单中选中的值设置修改哪些属性值
void chooseMode(GLint menuIteemNum)
{
	switch (menuIteemNum)
	{
	case 0:
		indexA = &translateX; indexB = &translateY; indexC = &translateZ; indexD = NULL; break;
	case 1:
		indexA = &scaleX; indexB = &scaleY; indexC = &scaleZ; indexD = NULL; break;
	case 2:
		indexA = &rotateAngle; indexB = &rotateX; indexC = &rotateY; indexD = &rotateZ; break;
	case 3:
		indexA = &Theta; indexB = &aspect; indexC = &dnear; indexD = &dfar; break;
	case 4:
		indexA = &x_0; indexB = &y_0; indexC = &z_0; indexD = NULL; break;
	case 5:
		indexA = &xref; indexB = &yref; indexC = &zref; indexD = NULL; break;
	case 6:
		indexA = &V_X; indexB = &V_Y; indexC = &V_Z; indexD = NULL; break;
	default:
		indexA = NULL; indexB = NULL; indexC = NULL; indexD = NULL; break;
	}
}
//鼠标右击后选项修改键盘控制参数
void mouseFunc(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	glutCreateMenu(chooseMode);
	glutAddMenuEntry("translate", 0);
	glutAddMenuEntry("scale", 1);
	glutAddMenuEntry("rotate", 2);
	glutAddMenuEntry("perspective", 3);
	glutAddMenuEntry("glLookAt eye", 4);
	glutAddMenuEntry("glLookAt center", 5);
	glutAddMenuEntry("glLookAt up", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//初始化
void init()
{
	readFile();
	glClearColor(0.2, 0.2, 0.2, 0.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	//创建光源
	glEnable(GL_LIGHT0);

	float fAmbientColor[] = { 0.2f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbientColor);

	float fDiffuseColor[] = { 0.8f, 0.7f, 0.65f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuseColor);

	float fSpecularColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecularColor);

	float fPosition[] = {1.0f, 1.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, fPosition);

	
	float fspotDirection[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, fspotDirection);
	float f[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, f);
	
	GLfloat ambient_lightModel[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_lightModel);
	
	glEnable(GL_LIGHTING);

	display();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	//窗口相对屏幕的位置
	glutInitWindowPosition(0, 0);
	//窗口大小
	glutInitWindowSize(680, 680);
	//标题
	glutCreateWindow("图形学大作业");

	init();
	//显示函数
	glutDisplayFunc(&display);
	//控制
	glutKeyboardFunc(keyBoardFunc);
	glutMouseFunc(mouseFunc);
	//循环
	glutMainLoop();

	return 0;
}
