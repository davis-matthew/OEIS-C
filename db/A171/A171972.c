/*
 * A171972  Greatest integer k such that k/n^2 < sqrt(3).
 * https://oeis.org/A171972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2 * sqrt(3))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 987 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The integer sqrt routine is provided by the utils library */
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
    /* Convert the index to seq_t to avoid overflow in multiplication */
    seq_t x = (seq_t)n;

    /* n^2 */
    seq_t sq = x * x;

    /* n^4 = (n^2)^2 */
    seq_t n4 = sq * sq;

    /* 3 * n^4 */
    seq_t val = 3 * n4;

    /* a(n) = floor(sqrt(3 * n^4)) = floor(n^2 * sqrt(3)) */
    return floor_sqrt(val);
}
