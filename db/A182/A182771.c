/*
 * A182771  Beatty sequence for (6+sqrt(3))/3.
 * https://oeis.org/A182771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(6+sqrt(3))/3)
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
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
    /* Convert index to seq_t to avoid mixing types. */
    seq_t nn = (seq_t)n;

    /* Compute n^2. */
    seq_t sq = nn * nn;

    /* Multiply by 3: 3*n^2. */
    seq_t m = sq * 3;

    /* floor_sqrt(3*n^2)   */
    seq_t r = floor_sqrt(m);

    /* floor( floor_sqrt(3*n^2) / 3 ). */
    seq_t t = r / 3;

    /* 2*n + floor(...). */
    return nn * 2 + t;
}
