/*
 * A198081  a(n) = ceiling(n*sqrt(3)).
 * https://oeis.org/A198081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9168 (2026-11-01)
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
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t s  = (seq_t)3 * nn * nn;   /* s = 3 * n^2 */
    seq_t f  = floor_sqrt(s);         /* f = floor(n*sqrt(3)) */
    return f + 1;                     /* ceil(n*sqrt(3)) = floor + 1 for n>0 */
}
