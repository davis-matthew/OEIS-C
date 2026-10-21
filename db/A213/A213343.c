/*
 * A213343  1-quantum transitions in systems of N spin 1/2 particles, in
 *          columns by combination indices. Triangle read by rows, T(n, k) for
 *          n >= 1 and 0 <= k <= floor((n-1)/2).
 * https://oeis.org/A213343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 93, u64 n <= 313,
 *            u128 n <= 1170, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A213343 (1‑quantum transitions in N spin‑1/2 particles)
   Term a(n) equals the triangle element
   T(1;N,k) = 2^(N-1-2k) * C(N,k) * C(N-k,k+1),
   where the index n is mapped to row N and column k by reading the triangle
   row‑wise. */

#include <stdint.h>
#include <limits.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* cumulative(N) = total number of elements up to row N (inclusive) */
static inline idx_t cumulative(idx_t N)
{
    idx_t p = N >> 1;
    __uint128_t prod;
    if ((N & 1) == 0) {
        prod = (__uint128_t)p * (p + 1);
    } else {
        idx_t q = p + 1;
        prod = (__uint128_t)q * q;
    }
    if (prod > UINT64_MAX) return UINT64_MAX;
    return (idx_t)prod;
}

/* Find the smallest N such that cumulative(N) >= n */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = n * 2;
    if (hi < n) hi = UINT64_MAX;              /* overflow guard */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cumulative(mid) >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* below offset, undefined */
    idx_t N = find_row(n);
    idx_t prev_c = cumulative(N - 1);
    idx_t k = n - prev_c - 1;
    idx_t exp = N - 1 - 2 * k;

    seq_t res = pow2(exp);
    res = res * binomial(N, k);
    res = res * binomial(N - k, k + 1);
    return res;
}
