/*
 * A097506  Duplicate of A001951.
 * https://oeis.org/A097506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊n·√2⌋
 * Range:     i8 n <= 5, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented as a(n) = ⌊√(2⋅n²)⌋ using the provided floor_sqrt function. */

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
    /* Compute 2·n² and take its integer square root. */
    seq_t t = nn * nn * (seq_t)2;
    return floor_sqrt(t);
}
