/*
 * A182765  Beatty sequence for (6 + sqrt(2))/4.
 * https://oeis.org/A182765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ((6 + √2)/4) * n ) The formula is evaluated
 *            exactly using only integer arithmetic. It follows that for all
 *            integer n ≥ 1 a(n) can be computed as a(n) = (6*n + floor(
 *            √(2*n*n) )) / 4 where the square root is the integer floor of
 *            the non‑negative integer 2*n*n. No floating point is re...
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
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
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index into the seq_t type. */
    seq_t nn = (seq_t)n;

    /* Compute floor(√(2*n*n)) = floor(n*√2). */
    seq_t sq = floor_sqrt( (seq_t)2 * nn * nn );

    /* a(n) = (6*n + sq) / 4, integer division is the floor. */
    return ( (seq_t)6 * nn + sq ) / 4;
}
