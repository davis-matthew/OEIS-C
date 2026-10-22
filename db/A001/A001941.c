/*
 * A001941  Absolute values of coefficients of an elliptic function.
 * https://oeis.org/A001941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} (1 - x^k)^(-c(k)), c(k) = 7 if 4 does not
 *            divide k, else 0. Big build: n*a(n) = Sum_{k=1..n} b(k)*a(n-k),
 *            a(0) = 1, b(k) = 7*Sum_{d|k, 4 does not divide d} d. Fixed-width
 *            build: expand the product directly using only additions
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 25, u64 n <= 81, u128 n <= 280,
 *            big any n
 * Helpers:   a001941_big_mode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a001941_big_mode.h"

static seq_t *a001941_A = NULL;   /* big mode: terms a(0..An-1) */
static idx_t a001941_An = 0;
static idx_t *a001941_B = NULL;   /* b(k) for k < Bcap */
static idx_t a001941_Bcap = 0;

static seq_t *a001941_F = NULL;   /* fixed mode: coefficients 0..Fn-1 */
static idx_t a001941_Fn = 0;

static void a001941_build_b(idx_t cap) {
    idx_t d, m;
    idx_t *b = (idx_t *)calloc(cap, sizeof(idx_t));
    for (d = 1; d < cap; d++) {
        if (d % 4 == 0) continue;
        for (m = d; m < cap; m += d) b[m] += 7 * d;
    }
    free(a001941_B);
    a001941_B = b;
    a001941_Bcap = cap;
}

static seq_t a001941_big(idx_t n) {
    if (n >= a001941_An) {
        idx_t need = n + 1, m, k, i;
        idx_t cap = a001941_An * 2;
        seq_t *na;
        if (cap < need) cap = need;
        if (a001941_Bcap < cap) a001941_build_b(cap);
        na = (seq_t *)calloc(cap, sizeof(seq_t));
        for (i = 0; i < a001941_An; i++) na[i] = a001941_A[i];
        free(a001941_A);
        a001941_A = na;
        if (a001941_An == 0) {
            a001941_A[0] = 1;
            a001941_An = 1;
        }
        for (m = a001941_An; m < cap; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += a001941_A[m - k] * a001941_B[k];
            a001941_A[m] = s / m;
        }
        a001941_An = cap;
    }
    return a001941_A[n];
}

static seq_t a001941_fixed(idx_t n) {
    if (n >= a001941_Fn) {
        idx_t N = a001941_Fn * 2, k, i;
        int r;
        seq_t *f;
        if (N < n + 1) N = n + 1;
        f = (seq_t *)calloc(N, sizeof(seq_t));
        f[0] = 1;
        for (k = 1; k < N; k++) {
            if (k % 4 == 0) continue;
            for (r = 0; r < 7; r++)
                for (i = k; i < N; i++) f[i] += f[i - k];
        }
        free(a001941_F);
        a001941_F = f;
        a001941_Fn = N;
    }
    return a001941_F[n];
}

seq_t a(idx_t n) {
    if (a001941_big_mode()) return a001941_big(n);
    return a001941_fixed(n);
}
