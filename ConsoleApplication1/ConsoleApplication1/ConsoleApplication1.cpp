#include<graphics.h>
#include<malloc.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include <stdio.h>
//#include <iostream>
//using namespace std;
typedef struct SNAKE//定义蛇
{
	int xx;
	int yy;
	struct SNAKE* last;
	struct SNAKE* next;
}snake, * link;

link head,end;
char key = 'd';
char key_temp = '\0';
int food_x, food_y;
int sleep_time = 250;
void create_snake_list()//创建蛇
{
	head = (link)malloc(sizeof(snake));
	end = (link)malloc(sizeof(snake));
	head->last = NULL;
	head->next = end;
	end->last = head;
	end->next = NULL;
}
void create_snake_head(link head, link end)//创建蛇头
{
	link body1;
	body1 = (link)malloc(sizeof(snake));
	head->next = body1;
	end->last = body1;
	body1->last = head;
	body1->next = end;
	body1->xx = 15;
	body1->yy = 15;
}
//绘制蛇的身体的颜色
int draw_snake_color()
{
	int scolor;//设置函数返回接受变量

	wchar_t s[10];// 定义字符串缓冲区，并接收用户输入

	outtextxy(20, 20, _T("选择蛇身的颜色！"));
	outtextxy(20, 80, _T("按1,输出红色！"));
	outtextxy(310, 80, _T("按2,输出绿色！"));
	outtextxy(20, 140, _T("按3,输出紫色！"));
	outtextxy(310, 140, _T("按4,输出深绿！"));

	InputBox((LPTSTR)s, 10, _T("选择按键！"));//弹框输入

	int r = _wtoi(s);//转换为数字

	switch (r)//选择功能
	{
	case 1: scolor = RGB(204, 23, 23);                      break;
	case 2: scolor = RGB(0, 255, 44);                        break;
	case 3: scolor = RGB(206, 5, 203);                        break;
	case 4: scolor = RGB(49, 101, 95);                       break;
	default: scolor = RGB(41, 52, 52);                   break;
	}
	return scolor;
}

void draw_snake_list(link head, link end, int scolor)//绘制蛇
{
	link p;
	p = head->next;
	//绘制蛇的身体的颜色
	setfillcolor(scolor);

	while (p != end)
	{
		fillcircle((p->xx * 20) - 10, (p->yy * 20) - 10, 10);//fillcircle(x,y,z)
		p = p->next;
	}
}
void hide_snake_list(link head, link end)//把蛇已经离开的部分藏起来
{
	link p;
	p = head->next;
	setfillcolor(RGB(132, 112, 255));
	while (p != end)
	{
		fillcircle((p->xx * 20) - 10, (p->yy * 20) - 10, 10);
		p = p->next;
	}
}
void snake_move(link head, link end)//蛇移动
{
	link p, q;
	p = head->next;
	q = end->last;
	while (q->last != head)
	{
		q->xx = q->last->xx;
		q->yy = q->last->yy;
		q = q->last;
	}
	if (key == 'd')
		p->xx++;
	else if (key == 'a')
		p->xx--;
	else if (key == 's')
		p->yy++;
	else if (key == 'w')
		p->yy--;
	p = p->next;

}
void check_key(char key_t)//输入检测，蛇移动不可180度变换方向
{
	if (key_t == '\0')
		return;
	else if (key_t == 'a' && key != 'd')
		key = key_t;
	else if (key_t == 'd' && key != 'a')
		key = key_t;
	else if (key_t == 'w' && key != 's')
		key = key_t;
	else if (key_t == 's' && key != 'w')
		key = key_t;
}

int check_SnakeAndWall(link head, link end)//撞墙检测
{
	link p;
	p = head->next;
	if (p->xx <= 0 || p->xx > 30 || p->yy <= 0 || p->yy > 30)
		return 0;
	return 1;
}
int check_SnakeAndFood(link head, link end)//检测，创建的食物不可与蛇重合
{
	link p;
	p = head->next;
	while (p != end)
	{
		if (food_x == p->xx && food_y == p->yy)
			return 1;
		p = p->next;
	}
	return 0;
}
void create_food(link head, link end)//随机创建食物
{
	do {
		srand((unsigned)time(NULL));
		food_x = rand() % 30 + 1;
		food_y = rand() % 30 + 1;
	} while (check_SnakeAndFood(head, end) == 1);

}
void draw_food()//绘制食物
{
	setfillcolor(RGB(238, 238, 0));
	fillcircle((food_x * 20) - 10, (food_y * 20) - 10, 10);
}

int check_SnakeEatFood(link head, link end)//蛇吃食物检测
{
	link p;
	p = head->next;
	if (p->xx == food_x && p->yy == food_y)
		return 1;
	return 0;
}

void SnakeBodyAdd(link head, link end)//蛇身增长
{
	link body, p, q;
	body = (link)malloc(sizeof(snake));
	p = end->last;
	body->xx = food_x;
	body->yy = food_y;
	p->next = body;
	end->last = body;
	body->last = p;
	body->next = end;
	create_food(head, end);
}
int check_SnakeAndBody(link head, link end)//判定蛇是否吃到自己
{
	link p, q;
	p = head->next;
	q = end->last;
	while (q->last != head)
	{
		if (p->xx == q->xx && p->yy == q->yy)
			return 1;
		q = q->last;
	}
	return 0;
}
int main()
{
	int scolor;
	initgraph(600, 600);
	setbkcolor(RGB(132, 112, 255));
	setlinecolor(RGB(132, 112, 255));
	settextstyle(42, 20, _T("宋体"));
	scolor = draw_snake_color();
	cleardevice();      //在显示输出后，清除显示内容
	create_snake_list();
	create_snake_head(head, end);
	draw_snake_list(head, end, scolor);
	create_food(head, end);
	while (1) {
		draw_food();
		Sleep(sleep_time);
		hide_snake_list(head, end);
		if (_kbhit())
			key_temp = _getch();
		check_key(key_temp);
		snake_move(head, end);
		if (check_SnakeAndWall(head, end) == 0)
		{
			outtextxy(220, 280, _T("游戏结束！"));
			system("pause");
			return 0;
		}
		if (check_SnakeAndBody(head, end) == 1)
		{
			outtextxy(220, 280, _T("游戏结束！"));
			system("pause");
			return 0;
		}
		if (check_SnakeAndFood(head, end) == 1)
		{
			SnakeBodyAdd(head, end);
			create_food(head, end);
		}
		draw_snake_list(head, end, scolor);

	}
	return 0;
}

