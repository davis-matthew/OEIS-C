/*
 * A183573  a(n) = n + floor(sqrt(2n+1)).
 * https://oeis.org/A183573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(2*n + 1))
 * Range:     u8 n <= 31, u16 n <= 8085, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9895 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t = (nn << 1) + 1;   /* 2*n + 1, using shift for multiplication by 2 */
    seq_t s = floor_sqrt(t);   /* floor(sqrt(2*n + 1)) */
    return nn + s;             /* a(n) = n + floor(sqrt(2*n + 1)) */
}
