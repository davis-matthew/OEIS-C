/*
 * A195181  a(n) = 6*n - floor(3*n*sqrt(3)).
 * https://oeis.org/A195181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - floor(3*n*sqrt(3))
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 6208, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9059 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity floor(3*n*sqrt(3)) = floor(sqrt(27*n*n)) */

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
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;      /* n*n  */
    prod *= (seq_t)27;         /* 27*n*n */
    seq_t sq = floor_sqrt(prod);  /* floor(sqrt(27*n*n)) */
    return 6 * nn - sq;
}
