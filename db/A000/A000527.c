/*
 * A000527  Series-parallel numbers.
 * https://oeis.org/A000527
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 4(13+14S+3S^2)(1+S)/(1-S)^7 =
 *            4(13+27S+17S^2+3S^3)/(1-S)^7, a(n) = [x^(n-4)], where S = g.f.
 *            of A000084: S = Sum c(m) x^m, c(1) = 1, c(m) = 2 b(m) for m >=
 *            2, and b = A000669 is given by the Euler transform relation
 *            Prod_k (1-x^k)^(-b(k)) = 1 + Sum c(m) x^m, b(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 38,
 *            big any n
 * Helpers:   a527_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a527_zero.h"

static seq_t *a527_g = NULL;
static idx_t a527_len = 0;

static void a527_build(idx_t N) {
    seq_t *b = a527_zero(N + 1);
    seq_t *c = a527_zero(N + 1);
    seq_t *d = a527_zero(N + 1);
    seq_t *t = a527_zero(N + 1);
    seq_t *u = a527_zero(N + 1);
    seq_t *g = a527_zero(N + 1);
    idx_t m, k, j, i;
    int r;

    c[0] = 1;
    if (N >= 1) { b[1] = 1; c[1] = 1; d[1] = 1; }
    for (m = 2; m <= N; m++) {
        seq_t R = 0, e = 0;
        for (k = 1; k < m; k++) R += d[k] * c[m - k];
        for (j = 1; j <= m / 2; j++)
            if (m % j == 0) e += (seq_t)j * b[j];
        R += e;
        b[m] = R / (seq_t)m;
        c[m] = 2 * b[m];
        d[m] = e + (seq_t)m * b[m];
    }
    /* c now holds S (with c[0] treated as 0) */

    /* t = 17 + 3S */
    t[0] = 17;
    for (i = 1; i <= N; i++) t[i] = 3 * c[i];
    /* u = 27 + S*t */
    for (i = 1; i <= N; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++) s += c[j] * t[i - j];
        u[i] = s;
    }
    u[0] = 27;
    /* g = 4*(13 + S*u) */
    for (i = 1; i <= N; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++) s += c[j] * u[i - j];
        g[i] = 4 * s;
    }
    g[0] = 52;
    /* divide by (1-S) seven times, in place */
    for (r = 0; r < 7; r++) {
        for (i = 1; i <= N; i++) {
            seq_t s = g[i];
            for (j = 1; j <= i; j++) s += c[j] * g[i - j];
            g[i] = s;
        }
    }

    free(b); free(c); free(d); free(t); free(u);
    if (a527_g) free(a527_g);
    a527_g = g;
    a527_len = N + 1;
}

seq_t a(idx_t n) {
    idx_t k, N;
    if (n < 4) return 0;
    k = n - 4;
    if (k >= a527_len) {
        N = k;
        if (N < 2 * a527_len) N = 2 * a527_len;
        if (N < 32) N = 32;
        a527_build(N);
    }
    return a527_g[k];
}
