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
 * Helpers:   alloc_zero, mul_trunc, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/mul_trunc.h"

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
