/*
 * A002508  Expansion of a modular function for Gamma_0(6).
 * https://oeis.org/A002508
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f.: eta(z)^2*eta(6z)^22/(eta(2z)^10*eta(3z)^14) = q^3 *
 *            Product_{k>=1} (1-x^k)^(-b(k)), the Euler transform of the
 *            period-6 sequence b = [-2, 8, 12, 8, -2, 0, ...]. Fixed-width
 *            builds multiply the series out factor by factor;
 *            arbitrary-precision builds use a(m) = (1/m) * Sum_{k=1..m}
 *            c(k)*a(m-k...
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 43, i64 n <= 121,
 *            i128 n <= 394, big any n
 * Helpers:   a002508_isbig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/a002508_isbig.h"

static seq_t *a002508_fx = NULL;
static idx_t a002508_fxlen = 0;

static void a002508_fixed_build(idx_t N) {
    idx_t k, j;
    int t;
    seq_t *f = (seq_t *)malloc(N * sizeof(seq_t));
    for (j = 0; j < N; j++) f[j] = 0;
    f[0] = 1;
    for (k = 1; k < N; k++) {
        int r = (int)(k % 6);
        if (r == 1 || r == 5) {
            for (t = 0; t < 2; t++)
                for (j = N - 1; j >= k; j--) f[j] -= f[j - k];
        } else if (r != 0) {
            int e = (r == 3) ? 12 : 8;
            for (t = 0; t < e; t++)
                for (j = k; j < N; j++) f[j] += f[j - k];
        }
    }
    free(a002508_fx);
    a002508_fx = f;
    a002508_fxlen = N;
}

static seq_t *a002508_ba = NULL;
static seq_t *a002508_bc = NULL;
static idx_t a002508_blen = 0;

static void a002508_big_build(idx_t N) {
    static const int bv[6] = {-2, 8, 12, 8, -2, 0};
    idx_t old = a002508_blen, i, k, d;
    long long *cc = (long long *)calloc(N, sizeof(long long));
    seq_t *na = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *nc = (seq_t *)malloc(N * sizeof(seq_t));
    for (d = 1; d < N; d++) {
        long long w = (long long)d * bv[(d - 1) % 6];
        if (w == 0) continue;
        for (k = d; k < N; k += d) cc[k] += w;
    }
    for (i = 0; i < N; i++) nc[i] = (seq_t)cc[i];
    free(cc);
    for (i = 0; i < old; i++) na[i] = a002508_ba[i];
    if (old == 0) { na[0] = 1; old = 1; }
    for (i = old; i < N; i++) {
        seq_t s = 0;
        for (k = 1; k <= i; k++) {
            if (cc == NULL) break;
            s += nc[k] * na[i - k];
        }
        na[i] = s / (seq_t)i;
    }
    free(a002508_ba);
    free(a002508_bc);
    a002508_ba = na;
    a002508_bc = nc;
    a002508_blen = N;
}

seq_t a(idx_t n) {
    static int mode = -1;
    idx_t m;
    if (n < 3) return 0;
    m = n - 3;
    if (mode < 0) mode = a002508_isbig();
    if (mode) {
        if (m >= a002508_blen) {
            idx_t N = a002508_blen * 2;
            if (N < m + 1) N = m + 1;
            if (N < 64) N = 64;
            a002508_big_build(N);
        }
        return a002508_ba[m];
    } else {
        if (m >= a002508_fxlen) {
            idx_t N = a002508_fxlen * 2;
            if (N < m + 1) N = m + 1;
            if (N < 64) N = 64;
            a002508_fixed_build(N);
        }
        return a002508_fx[m];
    }
}
