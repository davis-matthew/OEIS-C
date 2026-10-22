/*
 * A001884  Hit polynomials.
 * https://oeis.org/A001884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations p of {1..n} with exactly one i
 *            such that |p(i)-i| <= 1. a(n) = Sum_{k} (-1)^(k-1) * k * r_k(n)
 *            * (n-k)!, where r_k(n) are the rook numbers of the tridiagonal
 *            board: Sum_n R_n(x) y^n = (1-x*y)/(1-(1+2x)y-x*y^2+x^3*y^3).
 *            With q = t-1, F(y) = Sum_n H_n(q) y^n, H_n(q) =...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   a1884_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a1884_add.h"

#define A1884_NY 12
#define A1884_NQ 24

typedef struct { long long c[A1884_NY][A1884_NQ]; } a1884_p;

static a1884_p a1884_zero(void) {
    a1884_p r;
    memset(&r, 0, sizeof r);
    return r;
}

static a1884_p a1884_mono(int dy, int dq, long long c) {
    a1884_p r = a1884_zero();
    r.c[dy][dq] = c;
    return r;
}

static a1884_p a1884_mul(a1884_p x, a1884_p y) {
    a1884_p r = a1884_zero();
    int i, j, k, l;
    for (i = 0; i < A1884_NY; i++)
        for (j = 0; j < A1884_NQ; j++) {
            if (x.c[i][j] == 0) continue;
            for (k = 0; i + k < A1884_NY; k++)
                for (l = 0; j + l < A1884_NQ; l++)
                    if (y.c[k][l] != 0) r.c[i + k][j + l] += x.c[i][j] * y.c[k][l];
        }
    return r;
}

static a1884_p a1884_lin(a1884_p x, a1884_p y, long long s) {
    a1884_p r;
    int i, j;
    for (i = 0; i < A1884_NY; i++)
        for (j = 0; j < A1884_NQ; j++) r.c[i][j] = x.c[i][j] + s * y.c[i][j];
    return r;
}

static void a1884_eval(a1884_p x, long long *val, long long *der) {
    int i, j;
    for (i = 0; i < A1884_NY; i++) {
        long long v = 0, d = 0;
        for (j = 0; j < A1884_NQ; j++) {
            long long sg = (j % 2 == 0) ? 1 : -1;
            v += x.c[i][j] * sg;
            if (j > 0) d += x.c[i][j] * (long long)j * (-sg);
        }
        val[i] = v;
        der[i] = d;
    }
}

static long long a1884_Lv[A1884_NY], a1884_Ld[A1884_NY];
static long long a1884_Kv[A1884_NY], a1884_Kd[A1884_NY];
static long long a1884_Sv[A1884_NY], a1884_Sd[A1884_NY];
static int a1884_ready = 0;

static void a1884_init(void) {
    a1884_p one = a1884_mono(0, 0, 1);
    a1884_p Y = a1884_mono(1, 0, 1);
    a1884_p Q = a1884_mono(0, 1, 1);
    a1884_p A = a1884_lin(one, a1884_mono(1, 1, 1), -1);
    a1884_p B = a1884_lin(one, a1884_mono(1, 1, 1), 1);
    a1884_p P = a1884_lin(a1884_lin(one, a1884_mono(1, 1, 1), -2), a1884_mono(3, 3, 1), 1);
    a1884_p dP = a1884_lin(a1884_mono(0, 1, -2), a1884_mono(2, 3, 3), 1);
    a1884_p U = a1884_lin(one, a1884_mono(1, 1, 1), 2);
    a1884_p M = a1884_lin(a1884_mul(U, P), a1884_mul(a1884_mul(Y, B), dP), -1);
    a1884_p Y2B2 = a1884_mul(a1884_mul(Y, Y), a1884_mul(B, B));
    a1884_p L = a1884_mul(a1884_mul(Y2B2, A), P);
    a1884_p T1 = a1884_lin(a1884_mul(Q, P), a1884_mul(A, dP), 1);
    a1884_p K = a1884_lin(a1884_mul(a1884_mul(A, M), a1884_lin(P, a1884_mul(Y, B), -1)),
                          a1884_mul(T1, Y2B2), -1);
    a1884_p S = a1884_mul(a1884_mul(A, A), M);
    a1884_eval(L, a1884_Lv, a1884_Ld);
    a1884_eval(K, a1884_Kv, a1884_Kd);
    a1884_eval(S, a1884_Sv, a1884_Sd);
    a1884_ready = 1;
}

static seq_t *a1884_V = NULL, *a1884_W = NULL;
static idx_t a1884_cnt = 0, a1884_cap = 0;

static void a1884_grow(idx_t need) {
    idx_t nc = a1884_cap * 2, i;
    seq_t *nv, *nw;
    if (nc < need) nc = need;
    if (nc < 64) nc = 64;
    nv = (seq_t *)malloc((size_t)nc * sizeof(seq_t));
    nw = (seq_t *)malloc((size_t)nc * sizeof(seq_t));
    memset(nv, 0, (size_t)nc * sizeof(seq_t));
    memset(nw, 0, (size_t)nc * sizeof(seq_t));
    for (i = 0; i < a1884_cnt; i++) {
        nv[i] = a1884_V[i];
        nw[i] = a1884_W[i];
    }
    free(a1884_V);
    free(a1884_W);
    a1884_V = nv;
    a1884_W = nw;
    a1884_cap = nc;
}

seq_t a(idx_t n) {
    idx_t m;
    if (!a1884_ready) a1884_init();
    if (n >= a1884_cap) a1884_grow(n + 1);
    for (m = a1884_cnt; m <= n; m++) {
        seq_t pv = 0, nv = 0, pw = 0, nw = 0;
        idx_t i;
        if (m < A1884_NY) {
            a1884_add(&pv, &nv, a1884_Sv[m], (seq_t)1);
            a1884_add(&pw, &nw, a1884_Sd[m], (seq_t)1);
        }
        for (i = 2; i < A1884_NY; i++) {
            idx_t j;
            long long f;
            if (m + 1 < i) break;
            j = m + 1 - i;
            if (j == 0) continue;
            f = (long long)j;
            a1884_add(&pv, &nv, a1884_Lv[i] * f, a1884_V[j]);
            a1884_add(&pw, &nw, a1884_Ld[i] * f, a1884_V[j]);
            a1884_add(&pw, &nw, a1884_Lv[i] * f, a1884_W[j]);
        }
        for (i = 1; i < A1884_NY; i++) {
            idx_t j;
            if (m < i) break;
            j = m - i;
            a1884_add(&pv, &nv, -a1884_Kv[i], a1884_V[j]);
            a1884_add(&pw, &nw, -a1884_Kd[i], a1884_V[j]);
            a1884_add(&pw, &nw, -a1884_Kv[i], a1884_W[j]);
        }
        a1884_V[m] = pv - nv;
        a1884_W[m] = pw - nw;
        a1884_cnt = m + 1;
    }
    return a1884_W[n];
}
