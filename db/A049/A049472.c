/*
 * A049472  a(n) = floor(n/sqrt(2)).
 * https://oeis.org/A049472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 67, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 10000 (2026-11-01)
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
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    /* tmp = 2 * n * n, computed in seq_t to avoid overflow of idx_t */
    seq_t tmp = (seq_t)2 * nn * nn;
    seq_t sq = floor_sqrt(tmp);   /* floor(n * sqrt(2)) */
    return sq / 2;                /* floor((n * sqrt(2))/2) = floor(n/sqrt(2)) */
}
