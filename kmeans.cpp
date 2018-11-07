// kmeans.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream> 
#include <cstdio>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ------------------------------------
// param define
#define DCNT     200 /* ��ƭӼ�   */
#define DIM      3   /* ��ƺ���   */
#define K        5   /* �O�E�Ӽ�   */
#define MAX_ITER 30  /* �̤j���N   */
#define MIN_PT   0   /* �̤p�ܰ��I */
#define LOW      20  /* ��ƤU��   */
#define UP       300 /* ��ƤW��   */

// ------------------------------------
// variable
double data[DCNT][DIM]; /* ��l���   */
double cent[DCNT][DIM]; /* ����       */
double dis_k[K][DIM];   /* �O�E�Z��   */
int table[DCNT];        /* ��Ʃ����O�E*/
int cent_c[K];          /* ���O�E��Ƽ�*/
double cal_dis(double *x, double *y, int dim);
void   get_data();               // ���o���
void   kmeans_init();            // �t��k��l��
double update_table(int* ch_pt); // ��stable
void   update_cent();            // ��s���ߦ�m
void   print_cent();             // ��ܭ��ߦ�m
								 // ------------------------------------
								 // main function
int main()
{
	int     ch_pt;         /* �����ܰʤ��I */
	int     iter = 0;        /* ���N�p�ƾ�   */
	double sse1;           /* �W�@���N��sse */
	double sse2;           /* �������N��sse */

	srand((unsigned)time(NULL));
	get_data();                      /* step 0 - ���o���            */
	kmeans_init();                   /* step 1 - ��l��,�H�����o���� */
	sse2 = update_table(&ch_pt);     /* step 2 - ��s�@��������      */
	do {
		sse1 = sse2, ++iter;
		update_cent();             /* step 3 - ��s����            */
		sse2 = update_table(&ch_pt); /* step 4 - ��s������          */
	} while (iter<MAX_ITER && sse1 != sse2 && ch_pt>MIN_PT); // ���ı���

	print_cent(); // ��̫ܳ᭫�ߦ�m
	printf("sse   = %.2lf\n", sse2);
	printf("ch_pt = %d\n", ch_pt);
	printf("iter = %d\n", iter);
	system("pause");
	return 0;
}

// ------------------------------------
// �p��G�I�Z��
double cal_dis(double *x, double *y, int dim)
{
	int i;
	double t, sum = 0.0;
	for (i = 0; i<dim; ++i)
		t = x[i] - y[i], sum += t*t;
	return sum;
}

// ------------------------------------
// ���o��ơA���B�H�H����
void get_data()
{
	int i, j;
	for (i = 0; i<DCNT; ++i)
		for (j = 0; j<DIM; ++j)
			data[i][j] = \
			LOW + (double)rand()*(UP - LOW) / RAND_MAX;
}
// ------------------------------------
// �t��ƪ�l��
void   kmeans_init()
{
	int i, j, k, rnd;
	int pick[K];
	// �H����K �Ӥ��P����I
	for (k = 0; k<K; ++k) {
		rnd = rand() % DCNT; // �H�����@��
		for (i = 0; i<k && pick[i] != rnd; ++i);
		if (i == k) pick[k] = rnd; // �S����
		else --k; // ������, �A��@��
	}
	// �NK �Ӹ���I���e�ƨ�쭫��cent
	for (k = 0; k<K; ++k)
		for (j = 0; j<DIM; ++j)
			cent[k][j] = data[pick[k]][j];
}

// ------------------------------------
// ��stable, �Ǧ^sse, �s�J�I���ܰʼ�
double update_table(int* ch_pt)
{
	int i, j, k, min_k;
	double dis, min_dis, t_sse = 0.0;

	*ch_pt = 0;                          // �ܰ��I�Ƴ]0
	memset(cent_c, 0, sizeof(cent_c)); // �U�O�E��ƼƲM0
	memset(dis_k, 0, sizeof(dis_k));   // �U�O�E�Z���M�M0

	for (i = 0; i<DCNT; ++i) {
		// �M����ݭ���
		min_dis = cal_dis(data[i], cent[0], DIM);
		min_k = 0;
		for (k = 1; k<K; ++k) {
			dis = cal_dis(data[i], cent[k], DIM);
			if (dis < min_dis)
				min_dis = dis, min_k = k;
		}
		*ch_pt += (table[i] != min_k); // ��s�ܰ��I��
		table[i] = min_k;          // ��s���ݭ���
		++cent_c[min_k];           // �֭p���߸�Ƽ�         
		t_sse += min_dis;          // �֭p�`���߶Z��
		for (j = 0; j<DIM; ++j)       // ��s�U�O�E�`�Z��
			dis_k[min_k][j] += data[i][j];
	}
	return t_sse;
}

// ------------------------------------
// ��s���ߦ�m
void update_cent()
{
	int k, j;
	for (k = 0; k<K; ++k)
		for (j = 0; j<DIM; ++j)
			cent[k][j] = dis_k[k][j] / cent_c[k];
}

// ------------------------------------
// ��ܭ��ߦ�m
void   print_cent()
{
	int j, k;
	for (k = 0; k<K; ++k) {
		for (j = 0; j<DIM; ++j)
			printf("%6.2lf ", cent[k][j]);
		putchar('\n');
	}
}
