/*
 * A198266  a(n) = ceiling(n*sqrt(11)).
 * https://oeis.org/A198266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(11))
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 9402, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9402 (2026-11-01)
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
        return 0;                       /* a(0) = 0 by definition */

    seq_t nn = (seq_t)n;                 /* cast index to seq_t */
    /* v = 11 * n^2  (no overflow for the ranges of interest) */
    seq_t v = 11 * (nn * nn);
    /* floor_sqrt returns floor(sqrt(v)) = floor(n*sqrt(11)) */
    seq_t s = floor_sqrt(v);
    return s + 1;                        /* ceil(n*sqrt(11)) = floor + 1 */
}
