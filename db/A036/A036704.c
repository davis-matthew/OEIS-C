/*
 * A036704  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2.
 * https://oeis.org/A036704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057655(n^2+n)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9845 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* = number of Gaussian integers z = a+bi with |z| ≤ n + 1/2 */
/* = number of integer pairs (a,b) with a^2 + b^2 ≤ n^2 + n */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* m = n^2 + n */
    seq_t m = (seq_t)n * ((seq_t)n + (seq_t)1);

    /* max_a = floor(sqrt(m)) */
    seq_t max_a_seq = floor_sqrt(m);
    idx_t max_a = (idx_t)max_a_seq;   /* fits in idx_t (≤ ~1e4 for n≤1e4) */

    seq_t count = 0;
    for (idx_t a = 0; a <= max_a; ++a) {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        seq_t bmax = floor_sqrt(m - a_sq);
        seq_t pairs = (seq_t)2 * bmax + (seq_t)1;

        if (a == 0)
            count += pairs;
        else
            count += (seq_t)2 * pairs;   /* for +a and -a */
    }
    return count;
}
