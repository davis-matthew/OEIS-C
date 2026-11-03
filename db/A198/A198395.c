/*
 * A198395  a(n) = ceiling(n*sqrt(17)).
 * https://oeis.org/A198395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(17))
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 7280, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8522 (2026-11-01)
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
        return 0;
    seq_t nn = (seq_t)n;
    seq_t m = nn * nn * 17;          /* m = 17*n^2 */
    seq_t f = floor_sqrt(m);         /* floor(sqrt(17*n^2)) = floor(n*sqrt(17)) */
    return f + 1;                    /* ceil(n*sqrt(17)) = floor + 1 for n>0 */
}
