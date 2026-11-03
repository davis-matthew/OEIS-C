/*
 * A198083  Ceiling(n*Sqrt(6)).
 * https://oeis.org/A198083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n * sqrt(6))
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9781 (2026-11-01)
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
    if (n == 0)
        return (seq_t)0;
    seq_t sq = (seq_t)n;
    sq *= sq;          /* n * n */
    sq *= (seq_t)6;    /* 6 * n * n */
    seq_t m = floor_sqrt(sq); /* floor(sqrt(6*n*n)) */
    return m + (seq_t)1;      /* ceil(n*sqrt(6)) */
}
