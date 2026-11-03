/*
 * A003782  Coefficients of Jacobi Eisenstein series of index 1 and weight 6.
 * https://oeis.org/A003782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. t3^3*(t3^8 - 11*t3^4*t2^4/8 + 11*t2^8/32), with t3 = Sum_k
 *            q^(k^2) and t2 = Sum_k q^((k+1/2)^2) = 2*q^(1/4)*psi(q^2),
 *            psi(x) = Sum_{m>=0} x^(m(m+1)/2). Since t2^4 = 16*q*psi(q^2)^4,
 *            with P = psi(q^2)^4 this is a(n) = [q^n] t3^11 - 22*[q^(n-1)]
 *            t3^7*P + 88*[q^(n-2)] t3^3*P^2
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* dst = src * psi(q^2) = src * Sum_{m>=0} q^(m(m+1)), truncated to L terms */
static void mul_psi(const seq_t *src, seq_t *dst, idx_t L) {
    for (idx_t i = 0; i < L; i++) {
        seq_t s = 0;
        for (idx_t m = 0; m * (m + 1) <= i; m++)
            s += src[i - m * (m + 1)];
        dst[i] = s;
    }
}

/* dst = src * t3, truncated to L terms */
static void mul_t3(const seq_t *src, seq_t *dst, idx_t L) {
    for (idx_t i = 0; i < L; i++) {
        seq_t s = src[i];
        for (idx_t k = 1; k * k <= i; k++)
            s += 2 * src[i - k * k];
        dst[i] = s;
    }
}

/* returns series = delta * psi^np * t3^nt, length L */
static seq_t *make_series(idx_t L, int np, int nt) {
    seq_t *x = mem_new(seq_t, L);
    seq_t *y = mem_new(seq_t, L);
    seq_t *t;
    for (idx_t i = 0; i < L; i++) x[i] = 0;
    x[0] = 1;
    for (int j = 0; j < np; j++) { mul_psi(x, y, L); t = x; x = y; y = t; }
    for (int j = 0; j < nt; j++) { mul_t3(x, y, L); t = x; x = y; y = t; }
    mem_free(y);
    return x;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t L) {
    seq_t *A = make_series(L, 0, 11);
    seq_t *B = make_series(L, 4, 7);
    seq_t *C = make_series(L, 8, 3);
    seq_t *r = mem_new(seq_t, L);
    for (idx_t i = 0; i < L; i++) {
        seq_t v = A[i];
        if (i >= 1) v -= 22 * B[i - 1];
        if (i >= 2) v += 88 * C[i - 2];
        r[i] = v;
    }
    mem_free(A); mem_free(B); mem_free(C);
    mem_free(cache);
    cache = r;
    cache_len = L;
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t L = 2 * cache_len;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    return cache[n];
}
