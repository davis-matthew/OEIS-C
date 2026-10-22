/*
 * A006664  Number of irreducible systems of meanders.
 * https://oeis.org/A006664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   A(x) = S(x)#inv(x*S(x)^2), S = g.f. of A001246 (squared Catalan
 *            numbers). Big build: coefficients of the ODE A^2*(A^2-A-4x) -
 *            x*A*(3A^2-6A+8x)*A' + 4x^2*(A^2-3A+4x)*A'^2 - 2x^3*(A-4)*A'^3 +
 *            x^2*A*(A^2-16x)*A'' = 0 give (n+1)^2*a(n) = -(terms in
 *            a(0..n-1)), a(0) = 1. Fixed-width build: S(z) = A(z*...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, fin, regrow_c, release, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"
#include "../utils/fin.h"
#include "../utils/regrow_c.h"

typedef unsigned __int128 u128t;

static int is_fixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x *= 2;
    return x == 0;
}

/* exact modulo 2^128, using only + - * */
static seq_t small_a(idx_t n) {
    idx_t L = n + 1, k, m, j;
    u128t *c = (u128t *)calloc(L, sizeof(u128t));
    u128t *s = (u128t *)calloc(L, sizeof(u128t));
    u128t *uu = (u128t *)calloc(L, sizeof(u128t));
    u128t *pw = (u128t *)calloc(L, sizeof(u128t));
    u128t *tmp = (u128t *)calloc(L, sizeof(u128t));
    u128t *acc = (u128t *)calloc(L, sizeof(u128t));
    u128t ak = 0, t, *sw;
    c[0] = 1;
    for (m = 1; m <= n; m++) {
        t = 0;
        for (j = 0; j < m; j++) t += c[j] * c[m - 1 - j];
        c[m] = t;
    }
    for (m = 0; m <= n; m++) s[m] = c[m] * c[m];
    for (m = 1; m <= n; m++) {
        t = 0;
        for (j = 0; j < m; j++) t += s[j] * s[m - 1 - j];
        uu[m] = t;
    }
    pw[0] = 1;
    for (k = 0;; k++) {
        ak = s[k] - acc[k];
        if (k == n) break;
        for (m = k + 1; m <= n; m++) acc[m] += ak * pw[m];
        for (m = 0; m <= n; m++) tmp[m] = 0;
        for (m = k + 1; m <= n; m++) {
            t = 0;
            for (j = 1; j <= m - k; j++) t += uu[j] * pw[m - j];
            tmp[m] = t;
        }
        sw = pw; pw = tmp; tmp = sw;
    }
    free(c); free(s); free(uu); free(pw); free(tmp); free(acc);
    return u128_to_seq(ak);
}

static seq_t *A, *B, *C, *B2, *B3, *H1, *H2, *H3;
static char *S1, *S2, *S3;
static idx_t cap = 0, done = 0;

static seq_t *regrow(seq_t *old, idx_t oldcap, idx_t newcap, idx_t used) {
    seq_t *p = alloc_zero(newcap);
    idx_t i;
    for (i = 0; i < used; i++) p[i] = old[i];
    if (old) release(old, oldcap);
    return p;
}

static void step(idx_t n) {
    seq_t b2 = 0, b3 = 0, p2 = 0, q2 = 0, p3 = 0, q3 = 0, p4 = 0, q4 = 0;
    seq_t t, c1, hp, hq, ep, eq, an;
    idx_t i, k, nn;
    for (i = 1; i < n; i++) {
        seq_t x = A[n - i];
        b2 += B[i] * B[n - i];
        b3 += B2[i] * B[n - i];
        t = H1[i] * x; if (S1[i]) q2 += t; else p2 += t;
        t = H2[i] * x; if (S2[i]) q3 += t; else p3 += t;
        t = H3[i] * x; if (S3[i]) q4 += t; else p4 += t;
    }
    B2[n] = b2;
    B3[n] = b3;
    c1 = B[n - 1] * 8 + b2 * 12 + b3 * 2 + C[n - 1] * 16;
    /* tentative evaluation with a(n) = 0 */
    hp = p2 + b2 * 4;
    hq = q2;
    if (n == 1) hq += 4;
    ep = p4 + p3 + hp + B2[n - 1] * 16 + b3 * 8;
    eq = q4 + q3 + hq + c1;
    nn = n + 1;
    an = (eq - ep) / ((seq_t)nn * nn);
    A[n] = an;
    B[n] = an * n;
    C[n] = an * n * (n - 1);
    k = n * n + 1;
    if (k >= 4 * n) { H1[n] = an * (k - 4 * n); S1[n] = 0; }
    else { H1[n] = an * (4 * n - k); S1[n] = 1; }
    hp = p2 + b2 * 4 + B[n] * 6;
    hq = q2;
    if (n == 1) hq += 4;
    if (S1[n]) hq += H1[n]; else hp += H1[n];
    fin(hp, hq, &H2[n], &S2[n]);
    fin(p3 + hp, q3 + hq + c1, &H3[n], &S3[n]);
}

static void ensure(idx_t n) {
    idx_t m;
    if (n < done) return;
    if (n >= cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        A = regrow(A, cap, nc, done);
        B = regrow(B, cap, nc, done);
        C = regrow(C, cap, nc, done);
        B2 = regrow(B2, cap, nc, done);
        B3 = regrow(B3, cap, nc, done);
        H1 = regrow(H1, cap, nc, done);
        H2 = regrow(H2, cap, nc, done);
        H3 = regrow(H3, cap, nc, done);
        S1 = regrow_c(S1, nc, done);
        S2 = regrow_c(S2, nc, done);
        S3 = regrow_c(S3, nc, done);
        cap = nc;
    }
    if (done == 0) {
        A[0] = 1;
        done = 1;
    }
    for (m = done; m <= n; m++) step(m);
    done = n + 1;
}

seq_t a(idx_t n) {
    if (is_fixed()) {
        if (n > 400) return 0; /* far beyond any 128-bit value */
        return small_a(n);
    }
    ensure(n);
    return A[n];
}
