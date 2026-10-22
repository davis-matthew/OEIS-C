/*
 * A005762  Coefficients of modular function G_4(tau).
 * https://oeis.org/A005762
 *
 * Offset:    -4    (a(-4) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = A005760(n)^2 - 11*A005761(n), computed as a(n) =
 *            [q^n](x^2 - 11*y + 34*x - 140), where x = q^-2 + 2q^-1 + 4 + 5q
 *            + ..., y = q^-3 + 3q^-2 + 7q^-1 + 12 + ... are the Weierstrass
 *            functions of X0(11): y^2 + y = x^3 - x^2 - 10x - 20, with q
 *            dx/dq = -(2y+1) f and q du/dq = -(6x^2-4x-20) f, u = 2...
 * Range:     i8 n <= 4, i16 n <= 27, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"

#ifdef __cplusplus
typedef seq_t work_t;
#else
typedef __int128 work_t;
#endif

static work_t *X, *U, *S, *W;   /* x_i at X[i+2], u_i at U[i+3], s=x^2 and w at [i+4] */
static idx_t *C;                /* coefficients of f */
static idx_t cap = 0, xmax = -3;

#define XI(i) X[(i) + 2]
#define UI(i) U[(i) + 3]
#define SI(i) S[(i) + 4]
#define WI(i) W[(i) + 4]

static work_t *wgrow(work_t *p, idx_t o, idx_t nl)
{
#ifdef __cplusplus
    return grow_zero(p, o, nl);
#else
    work_t *q = (work_t *)realloc(p, (size_t)nl * sizeof(work_t));
    idx_t i;
    for (i = o; i < nl; i++) q[i] = 0;
    return q;
#endif
}

static void build_c(idx_t L)
{
    idx_t *e = (idx_t *)calloc((size_t)L, sizeof(idx_t));
    idx_t *e2 = (idx_t *)calloc((size_t)L, sizeof(idx_t));
    idx_t *g = (idx_t *)calloc((size_t)L, sizeof(idx_t));
    idx_t i, j, k, m;
    e[0] = 1;
    for (j = 1;; j++) {
        idx_t k1 = j * (3 * j - 1) / 2, k2 = j * (3 * j + 1) / 2;
        idx_t sg = (j & 1) ? -1 : 1;
        if (k1 >= L) break;
        e[k1] += sg;
        if (k2 < L) e[k2] += sg;
    }
    for (i = 0; i < L; i++)
        if (e[i])
            for (k = i; k < L; k++) e2[k] += e[i] * e[k - i];
    for (m = 0; 11 * m < L; m++)
        if (e2[m])
            for (k = 11 * m; k < L; k++) g[k] += e2[m] * e2[k - 11 * m];
    C[0] = 0;
    for (k = 1; k < L; k++) C[k] = g[k - 1];
    free(e);
    free(e2);
    free(g);
}

static void ensure_cap(idx_t need)
{
    idx_t nc;
    if (need <= cap) return;
    nc = 2 * cap;
    if (nc < need) nc = need;
    X = wgrow(X, cap, nc);
    U = wgrow(U, cap, nc);
    S = wgrow(S, cap, nc);
    W = wgrow(W, cap, nc);
    C = (idx_t *)realloc(C, (size_t)nc * sizeof(idx_t));
    build_c(nc);
    cap = nc;
}

static void compute_sw(idx_t j)
{
    work_t s = 0, xj = 0;
    idx_t i;
    for (i = -2; i <= j + 2; i++) s += XI(i) * XI(j - i);
    SI(j) = s;
    if (j >= -2) xj = XI(j);
    WI(j) = 6 * s - 4 * xj;
    if (j == 0) WI(j) -= 20;
}

static void init_seeds(void)
{
    static const int xs0[7] = {1, 2, 4, 5, 8, 1, 7};
    static const int us0[7] = {2, 6, 14, 25, 34, 52, 38};
    idx_t i;
    for (i = 0; i < 7; i++) {
        X[i] = xs0[i];
        U[i] = us0[i];
    }
    for (i = -4; i <= 2; i++) compute_sw(i);
    xmax = 4;
}

static void step(idx_t N)
{
    work_t R1 = 0, R2 = 0, Sp = 0, Wp, num, uN, xN1;
    idx_t k, i;
    for (k = 2; k <= N + 4; k++)
        if (C[k]) R1 += UI(N + 1 - k) * C[k];
    for (k = 2; k <= N + 4; k++)
        if (C[k]) R2 += WI(N - k) * C[k];
    for (i = -1; i < N - 1 - i; i++) Sp += XI(i) * XI(N - 1 - i);
    Sp *= 2;
    if (((N - 1) & 1) == 0) Sp += XI((N - 1) / 2) * XI((N - 1) / 2);
    Wp = 6 * Sp - 4 * XI(N - 1);
    num = 12 * R1 - (Wp + R2) * (N + 1);
    uN = num / (N * N + N - 12);
    xN1 = -(uN + R1) / (N + 1);
    UI(N) = uN;
    XI(N + 1) = xN1;
    SI(N - 1) = Sp + 2 * xN1;
    WI(N - 1) = 6 * SI(N - 1) - 4 * XI(N - 1);
    xmax = N + 1;
}

seq_t a(idx_t n)
{
    work_t r, y, xn = 0, un = 0;
    if (n < -4) return 0;
    ensure_cap((n > 48 ? n : 48) + 16);
    if (xmax < 0) init_seeds();
    while (xmax < n + 2) step(xmax);
    if (n >= -2) xn = XI(n);
    if (n >= -3) un = UI(n);
    if (n == 0) un -= 1;
    y = un / 2;
    r = SI(n) - 11 * y + 34 * xn;
    if (n == 0) r -= 140;
    return (seq_t)r;
}
