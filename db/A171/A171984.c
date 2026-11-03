/*
 * A171984  Beatty sequence for sqrt(17).
 * https://oeis.org/A171984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sqrt(17))
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 973 (2026-11-01)
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
    /* compute floor(sqrt(17*n^2)) which equals floor(n*sqrt(17)) */
    seq_t nn = (seq_t)n * (seq_t)n;    /* n^2 */
    seq_t t  = nn * (seq_t)17;         /* 17*n^2 */
    return floor_sqrt(t);
}
