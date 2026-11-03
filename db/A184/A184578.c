/*
 * A184578  a(n) = floor((n+1/3)*sqrt(2)), complement of A184579.
 * https://oeis.org/A184578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1/3)*sqrt(2))
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 7264, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9726 (2026-11-01)
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
    seq_t k = (seq_t)3 * (seq_t)n + (seq_t)1;
    seq_t t = (seq_t)2 * k * k;
    seq_t sq = floor_sqrt(t);   /* floor(sqrt(2*k*k)) = floor(k*sqrt(2)) */
    return sq / (seq_t)3;       /* floor((k*sqrt(2))/3) = floor((n+1/3)*sqrt(2)) */
}
