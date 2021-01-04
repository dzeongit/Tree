#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
using namespace std;
typedef char TElemType;
typedef int Status;
#define ERROR 0;
#define TRUE 1;
#define FALSE 0;
#define OVERFLOW -1;
#define OK 1;
typedef struct CSTNode {
	TElemType data;
	struct CSTNode* firstChild, * nextSibling;
}CSTNode, * CSTree, * CSForest;//孩子兄弟表示法
typedef struct LQNode {
	CSTree data;
	struct LQNode* next;
}LQNode, * QueuePtr;//结点及其指针类型
typedef struct {
	QueuePtr front;//队头
	QueuePtr rear;//队尾
}LQueue;


Status InitQueue_LQ(LQueue& Q)//构造一个空队列
{
	Q.front = Q.rear = (LQNode*)malloc(sizeof(LQNode));
	Q.front->next = NULL;
	return OK;
}
Status QueueEmpty_LQ(LQueue Q) {//判断队列空与否
	if (Q.rear == Q.front)return TRUE;
	return FALSE;
}
Status EnQueue_LQ(LQueue& Q, CSTree e) //插入元素e为Q的新队尾元素
{
	LQNode* p;
	p = (LQNode*)malloc(sizeof(LQNode));
	if (NULL == p)return OVERFLOW;
	p->data = e;
	p->next = NULL;
	//if (NULL == Q.front)Q.front = p;
	/*if (QueueEmpty_LQ(Q) == 1)Q.front = p;
	else Q.rear->next = p;*/
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}
Status DeQueue_LQ(LQueue& Q, CSTree& e) //删除Q的队头元素
{
	/*LQNode* p;
	if (NULL == Q.front) return ERROR;
	p = Q.front;
	e = p->data;
	Q.front = p->next;
	if (Q.rear == p)Q.rear = NULL;
	free(p);
	return OK;*/
	if (Q.front == Q.rear)
	{
		return ERROR;
	}
	QueuePtr p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p)Q.rear = Q.front;
	free(p);
	return OK;
}

Status InitTree(CSTree& T) {
	if (NULL == T) { return ERROR; }
	else
	{
		T->firstChild = NULL;
		T->nextSibling = NULL;
	}
}//创建空树T
CSTree CreateTree(char* defCS, int& i) {
	CSTree T;
	TElemType ch;
	ch = defCS[i++];
	if ('#' == ch)T = NULL;
	else {
		T = (CSTree)malloc(sizeof(CSTNode));
		T->data = ch;
		T->firstChild = CreateTree(defCS, i);
		T->nextSibling = CreateTree(defCS, i);
	}
	return T;
}
CSTree MakeTree(TElemType e, int n, ...) {//创建根节点e和n棵子树的树
	int i;
	CSTree t, p, pi;
	va_list argptr;
	t = (CSTree)malloc(sizeof(CSTNode));
	if (NULL == t)return NULL;
	t->data = e;
	t->firstChild = t->nextSibling = NULL;
	if (n <= 0)return t;
	va_start(argptr, n);
	p = va_arg(argptr, CSTree);
	t->firstChild = p;
	pi = p;
	for (i = 1; i < n; i++) {
		p = va_arg(argptr, CSTree);
		pi->nextSibling = p;
		pi = p;
	}
	va_end(argptr);
	return t;
}
Status DestroyTree(CSTree& T) {
	if (NULL != T)
	{
		DestroyTree(T->firstChild);
		DestroyTree(T->nextSibling);
		free(T);
		return OK;
	}
	return ERROR;
}//销毁树
int TreeDepth(CSTree T) {//返回树的深度
	/*int a, b, c;
	CSTree p;
	if (NULL == T)return 0;
	else {
		p = T->firstChild;
		a = 0;
		b = 0;
		while (p) {
			a++;
			c = TreeDepth(p);
			p = p->nextSibling;
			b = b > c ? b : c;
		}
		return a > b ? a : b;

	}*/
	int d, dc, ds;
	if (NULL == T)
		d = 0;
	else
	{
		dc = TreeDepth(T->firstChild);
		ds = TreeDepth(T->nextSibling);
		d = dc + 1 > ds ? dc + 1 : ds;
	}
	return d;
}
CSTNode* Search(CSTree T, TElemType e) {
	if (NULL == T)return NULL;
	if (T->data == e)return T;
	if (T->firstChild == NULL && T->nextSibling == NULL)return NULL;
	else
	{
		if (T->firstChild != NULL)
		{
			CSTree TTemp = Search(T->firstChild, e);
			if (TTemp != NULL)return TTemp;
			else
			{
				return Search(T->nextSibling, e);
			}
		}
		else
			return Search(T->nextSibling, e);
	}
}
Status InsertChild(CSTree& T, int i, CSTree c) {//插入c为T的第i棵子树，c非空并于T不相交
	int j;
	CSTree p;
	if (NULL == T || i < 1)return ERROR;
	if (1 == i) {
		c->nextSibling = T->firstChild;
		T->firstChild = c;
	}
	else {
		p = T->firstChild;
		for (j = 2; p != NULL && j < i; i++)p = p->nextSibling;
		if (j == i) {
			c->nextSibling = p->nextSibling;
			p->nextSibling = c;
		}
		else return ERROR;
	}
	return OK;
};
Status DeleteChild(CSTree& T, int i) {
	int j;
	CSTree p;
	if (NULL == T || i < 1)return ERROR;
	if (1 == i) {
		DestroyTree(T->firstChild);
	}
	else {
		p = T->firstChild;
		for (j = 2; p != NULL && j < i; i++)p = p->nextSibling;
		if (j == i) {
			DestroyTree(p->nextSibling);
		}
		else return ERROR;
	}
	return OK;
}//删除T的第i棵子树
void CountLeaf(CSTree T, int& count)//叶子节点计数
{
	if (NULL != T)
	{
		if (NULL == T->firstChild) count++;
		CountLeaf(T->firstChild, count);
		CountLeaf(T->nextSibling, count);
	}
}
void PrintLeaf(CSTree T)
{
	if (NULL != T)
	{
		if (NULL == T->firstChild) printf("%c", T->data);
		PrintLeaf(T->firstChild);
		PrintLeaf(T->nextSibling);
	}
}
void PreorderTraversal(CSTree T) {
	if (NULL != T) {
		printf("%c", T->data);
		PreorderTraversal(T->firstChild);
		PreorderTraversal(T->nextSibling);
	}
}
void PostorderTraversal(CSTree T) {
	if (NULL != T) {
		PostorderTraversal(T->firstChild);
		printf("%c", T->data);
		PostorderTraversal(T->nextSibling);

	}
}
void LevelTraversal(CSTree T) {
	LQueue Q;
	//CSTree c, p;
	CSTNode* c, * p;
	if (T != NULL) {
		InitQueue_LQ(Q);
		EnQueue_LQ(Q, T);
		while (0 == QueueEmpty_LQ(Q)) {
			DeQueue_LQ(Q, c);
			printf("%c", c->data);
			if (NULL != c->firstChild) {
				p = c->firstChild;
				EnQueue_LQ(Q, p);
				while (NULL != p->nextSibling) {
					p = p->nextSibling;
					EnQueue_LQ(Q, p);
				}
			}
		}
	}
}
void PrintTreeInfo(CSTree T) {
	printf("树的先根遍历输出为：\n");
	PreorderTraversal(T);
	printf("\n树的后根遍历输出为：\n");
	PostorderTraversal(T);
	printf("\n所有叶子结点：\n");
	PrintLeaf(T);
	int a = 0;
	CountLeaf(T, a);
	printf("\n树的叶子结点个数为：%d\n", a);
	a = TreeDepth(T);
	printf("\n树的深度为：%d\n", a);
	printf("\n层次遍历：\n");
	LevelTraversal(T);
}
int main()
{
	int i = 0;
	int a;
	char tree1[] = "ABF#G##C#E###";
	CSTree s, j;
	s = CreateTree(tree1, i);
	PrintTreeInfo(s);
	j = MakeTree('U', 1, s);
	PrintTreeInfo(j);
	return 0;
}