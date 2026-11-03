/*
 * A135668  a(n) = ceiling(n + sqrt(n)).
 * https://oeis.org/A135668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n + sqrt(n))
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 925 (2026-11-01)
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
    /* Convert the index to seq_t for arithmetic with big integers. */
    seq_t xn = (seq_t)n;

    /* Compute the integer floor of the square root of n. */
    seq_t sqrt_floor = floor_sqrt(xn);

    /* Start with the sum n + floor(sqrt(n)). */
    seq_t res = xn + sqrt_floor;

    /* If sqrt(n) is not an integer, add 1 to achieve the ceiling. */
    if (sqrt_floor * sqrt_floor != xn)
        res += 1;

    return res;
}
