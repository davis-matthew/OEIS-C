/*
 * A063657  Numbers whose truncated square root is unequal to rounded square
 *          root.
 * https://oeis.org/A063657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (t*(t+3))/2 + n, where t = floor( sqrt(2*n) + 0.5 ) (i.e.
 *            t is the nearest integer to sqrt(2*n))
 * Range:     u8 n <= 31, u16 n <= 6163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9646 (2026-11-01)
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
    /* 2*n may overflow n's type, so use seq_t for the product */
    seq_t two_n = (seq_t)n * 2;
    /* r = floor(sqrt(2*n)) */
    seq_t r = floor_sqrt(two_n);
    /* Determine whether to round up: if fractional part >= 0.5 */
    seq_t diff = two_n - r * r;
    seq_t t = r;
    if (diff >= r + 1)
        t = r + 1;
    /* a(n) = (t^2 + 3t)/2 + n  -->  (t*(t+3))/2 + n  */
    return (t * (t + 3)) / 2 + n;
}
