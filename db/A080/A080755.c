/*
 * A080755  a(n) = ceiling(n*(1+1/sqrt(2))).
 * https://oeis.org/A080755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(1+1/sqrt(2))) = n + floor(n/√2) + 1
 * Range:     u8 n <= 11, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9922 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Convert index to seq_t */
    seq_t n_seq = (seq_t)n;
    /* Compute floor(n/√2) as floor_sqrt(n^2 / 2) */
    seq_t sq = n_seq * n_seq;
    seq_t half_sq = sq / (seq_t)2;
    seq_t v = floor_sqrt(half_sq);
    /* a(n) = n + v + 1 */
    return n_seq + v + (seq_t)1;
}
