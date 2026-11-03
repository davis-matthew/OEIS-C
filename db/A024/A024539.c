/*
 * A024539  a(n) = [ 1/{n*sqrt(2)} ], where {x} := x - [ x ].
 * https://oeis.org/A024539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ 1 / frac(n*√2) ⌋ Equivalent to: a(n) = ⌊ (2*⌊√(2*n²)⌋)
 *            / (2*n² - ⌊√(2*n²)⌋²) ⌋ where frac(x) = x - ⌊x⌋. This uses only
 *            integer arithmetic
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 116 terms through n = 888 (2026-11-01)
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
    /* Convert index to seq_t type. */
    seq_t nn = (seq_t)n;

    /* M = 2 * n² */
    seq_t m2 = nn * nn;
    seq_t M   = m2 + m2;             /* 2 * n² */

    /* m = floor( sqrt(M) ) = floor( n * sqrt(2) ) */
    seq_t m = floor_sqrt(M);

    /* delta = M - m²  (positive for n >= 1) */
    seq_t delta = M - m * m;

    /* numerator = 2 * m */
    seq_t numerator = m + m;

    /* Result = floor(numerator / delta) */
    seq_t result = numerator / delta;

    return result;
}
