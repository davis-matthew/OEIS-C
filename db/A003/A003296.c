/*
 * A003296  Coefficients of modular function g_2(tau).
 * https://oeis.org/A003296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   g_2 is the modular function on Gamma_0(11) with a double pole at
 *            the cusp 0 and a simple zero at infinity: g_2 = (E_4(q) -
 *            E_4(q^11) - 2064*f^2 - 48*f*E')/(14640*f^2), where f =
 *            eta(tau)^2*eta(11*tau)^2 and E' = (11*E_2(q^11) - E_2(q))/2 = 5
 *            + 12*Sum (sigma(k) - 11*sigma(k/11))*q^k. So a(n) = [q^...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   A_grow, a3296_div, a3296_isfixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A_grow.h"
#include "../utils/a3296_div.h"
#include "../utils/a3296_isfixed.h"

static seq_t *a3296_ym = NULL;
static unsigned char *a3296_yn = NULL;
static idx_t a3296_have = 0;

static void a3296_build(idx_t L)
{
    idx_t S = L + 3, k, i, j;
    long long *e1 = (long long *)calloc(S, sizeof(long long));
    long long *e3 = (long long *)calloc(S, sizeof(long long));
    long long *e2 = (long long *)calloc(S, sizeof(long long));
    long long *e4 = (long long *)calloc(S, sizeof(long long));
    long long *F = (long long *)calloc(S, sizeof(long long));
    long long *f = (long long *)calloc(S, sizeof(long long));
    long long *s1 = (long long *)calloc(S, sizeof(long long));
    long long *s3 = (long long *)calloc(S, sizeof(long long));
    long long *Ep = (long long *)calloc(S, sizeof(long long));
    long long *x1 = (long long *)malloc(sizeof(long long) * (S + 4));
    long long *c1 = (long long *)malloc(sizeof(long long) * (S + 4));
    long long *x3 = (long long *)malloc(sizeof(long long) * (S + 4));
    long long *c3 = (long long *)malloc(sizeof(long long) * (S + 4));
    int n1 = 0, n3 = 0, fx = a3296_isfixed();
    long long kk;

    for (kk = 0;; kk++) {
        long long p1 = kk * (3 * kk - 1) / 2, p2 = kk * (3 * kk + 1) / 2;
        long long sg = (kk & 1) ? -1 : 1;
        if (p1 >= (long long)S) break;
        e1[p1] += sg;
        if (p1 > 0) { x1[n1] = p1; c1[n1] = sg; n1++; }
        if (kk > 0 && p2 < (long long)S) { e1[p2] += sg; x1[n1] = p2; c1[n1] = sg; n1++; }
    }
    for (kk = 0; kk * (kk + 1) / 2 < (long long)S; kk++) {
        long long p = kk * (kk + 1) / 2;
        long long c = ((kk & 1) ? -1 : 1) * (2 * kk + 1);
        e3[p] = c;
        if (p > 0) { x3[n3] = p; c3[n3] = c; n3++; }
    }
    for (i = 0; i < S; i++) {
        if (e1[i] == 0 && e3[i] == 0) continue;
        for (j = 0; i + j < S; j++) {
            if (e1[j] == 0) continue;
            e2[i + j] += e1[i] * e1[j];
            e4[i + j] += e3[i] * e1[j];
        }
    }
    for (k = 0; k < S; k++) {
        long long sF = 0, sf = 0;
        idx_t t;
        for (t = 0; 11 * t <= k; t++) {
            sF += e4[t] * e4[k - 11 * t];
            sf += e2[t] * e2[k - 11 * t];
        }
        F[k] = sF;
        if (k + 1 < S) f[k + 1] = sf;
    }
    for (i = 1; i < S; i++)
        for (j = i; j < S; j += i) {
            s1[j] += (long long)i;
            s3[j] += (long long)i * (long long)i * (long long)i;
        }
    Ep[0] = 5;
    for (k = 1; k < S; k++)
        Ep[k] = 12 * s1[k] - ((k % 11 == 0) ? 132 * s1[k / 11] : 0);

    a3296_ym = A_grow(a3296_ym, a3296_have, L);
    a3296_yn = (unsigned char *)realloc(a3296_yn, L);
    for (j = 0; j < L; j++) {
        idx_t m = j + 3, t;
        long long fE = 0, N, U;
        unsigned long long mag;
        for (t = 1; t <= m; t++) fE += f[t] * Ep[m - t];
        N = 240 * s3[m] - ((m % 11 == 0) ? 240 * s3[m / 11] : 0) - 2064 * F[m - 2] - 48 * fE;
        U = N / 14640;
        mag = (unsigned long long)(U < 0 ? -U : U);
        if (U < 0) {
            if (fx) { a3296_ym[j] = (seq_t)0 - (seq_t)mag; a3296_yn[j] = 0; }
            else { a3296_ym[j] = (seq_t)mag; a3296_yn[j] = 1; }
        } else {
            a3296_ym[j] = (seq_t)mag; a3296_yn[j] = 0;
        }
    }
    a3296_div(a3296_ym, a3296_yn, L, x3, c3, n3, 1, fx);
    a3296_div(a3296_ym, a3296_yn, L, x1, c1, n1, 1, fx);
    a3296_div(a3296_ym, a3296_yn, L, x3, c3, n3, 11, fx);
    a3296_div(a3296_ym, a3296_yn, L, x1, c1, n1, 11, fx);
    a3296_have = L;

    free(e1); free(e3); free(e2); free(e4); free(F); free(f);
    free(s1); free(s3); free(Ep); free(x1); free(c1); free(x3); free(c3);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > a3296_have) {
        idx_t L = 2 * a3296_have;
        if (L < n) L = n;
        if (L < 64) L = 64;
        a3296_build(L);
    }
    return a3296_ym[n - 1];
}
