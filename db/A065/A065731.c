/*
 * A065731  Largest square <= n!.
 * https://oeis.org/A065731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(n!))^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 100 (2026-11-01)
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
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Integer square root of n! */
    seq_t s = floor_sqrt(fact);

    /* Return s^2 which is the largest square <= n! */
    return s * s;
}
