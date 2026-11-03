/*
 * A067456  Floor( sqrt( sum of the decimal digits of n squared)).
 * https://oeis.org/A067456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(sqrt( sum over decimal digits of n of (digit)^2 ))
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 97 (2026-11-01)
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
    idx_t tmp = n;
    seq_t sum = 0;          /* sum of squares of decimal digits of n */
    while (tmp > 0)
    {
        unsigned digit = (unsigned)(tmp % 10);
        sum += (seq_t)(digit * digit);
        tmp /= 10;
    }
    return floor_sqrt(sum);
}
