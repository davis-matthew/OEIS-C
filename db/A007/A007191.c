/*
 * A007191  McKay-Thompson series of class 2B for the Monster group with a(0)
 *          = -24.
 * https://oeis.org/A007191
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f.: (1/x)*(Product_{k>0} (1 - x^(2*k-1)))^24 =
 *            (eta(q)/eta(q^2))^24. Arbitrary precision: Euler transform
 *            recurrence m*b(m) = Sum_{j=1..m} c(j)*b(m-j), b(0) = 1, c(j) =
 *            -24*(sum of odd divisors of j), a(n) = b(n+1). Fixed width: P =
 *            Product_{k odd} (1 - x^k), then P^24 = P^16 * P^8 by repeated...
 * Range:     i8 n <= 0, i16 n <= 3, i32 n <= 14, i64 n <= 56, i128 n <= 217,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 4973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static void mul_trunc(seq_t *r, const seq_t *x, const seq_t *y, idx_t L) {
    idx_t i, j;
    for (i = 0; i < L; i++) r[i] = 0;
    for (i = 0; i < L; i++) {
        if (x[i] == 0) continue;
        for (j = 0; i + j < L; j++)
            r[i + j] += x[i] * y[j];
    }
}

static seq_t *cache = 0;
static idx_t clen = 0;

static seq_t *build(idx_t L) {
    seq_t *A = alloc_zero(L);
    idx_t i, k;
    if (seq_is_big()) {
        long long *sig = (long long *)calloc((size_t)L, sizeof(long long));
        idx_t d, m, j;
        for (d = 1; d < L; d += 2)
            for (m = d; m < L; m += d) sig[m] += d;
        A[0] = 1;
        for (m = 1; m < L; m++) {
            seq_t s = 0;
            for (j = 1; j <= m; j++)
                s += A[m - j] * (seq_t)(-24LL * sig[j]);
            A[m] = s / m;
        }
        free(sig);
    } else {
        seq_t *P = alloc_zero(L);
        seq_t *P2 = alloc_zero(L);
        seq_t *P4 = alloc_zero(L);
        seq_t *P8 = alloc_zero(L);
        seq_t *P16 = alloc_zero(L);
        P[0] = 1;
        for (k = 1; k < L; k += 2)
            for (i = L - 1; i >= k; i--)
                P[i] -= P[i - k];
        mul_trunc(P2, P, P, L);
        mul_trunc(P4, P2, P2, L);
        mul_trunc(P8, P4, P4, L);
        mul_trunc(P16, P8, P8, L);
        mul_trunc(A, P16, P8, L);
        release(P, L);
        release(P2, L);
        release(P4, L);
        release(P8, L);
        release(P16, L);
    }
    return A;
}

seq_t a(idx_t n) {
    idx_t idx;
    if (n < -1) return 0;
    idx = n + 1;
    if (idx >= clen) {
        idx_t L = idx + 1;
        if (L < 2 * clen) L = 2 * clen;
        if (L < 64) L = 64;
        if (cache) release(cache, clen);
        cache = build(L);
        clen = L;
    }
    return cache[idx];
}
