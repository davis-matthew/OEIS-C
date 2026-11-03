/*
 * A078588  a(n) = 1 if the integer multiple of phi nearest n is greater than
 *          n, otherwise 0, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A078588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + floor(sqrt(5*n*n))) mod 2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 122 terms through n = 992 (2026-11-01)
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
    seq_t temp = nn * nn * 5;                 /* 5*n^2  */
    seq_t s = floor_sqrt(temp);                /* floor(sqrt(5*n^2)) = floor(n*sqrt5) */
    seq_t sum = nn + s;
    return sum & 1;                            /* (n + floor(n*sqrt5)) mod 2 */
}
