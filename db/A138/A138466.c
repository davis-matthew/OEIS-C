/*
 * A138466  a(1)=1; for n >= 2, a(n) = n - floor((1/2)*a(a(n-1))).
 * https://oeis.org/A138466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((sqrt(3)-1)*(n+1)) = floor((n+1)*sqrt(3)) - (n+1)
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
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
    /* k = n + 1 */
    seq_t k = (seq_t)n + 1;

    /* t = 3 * k * k */
    seq_t t = k * k;
    t = t * (seq_t)3;

    /* floor sqrt of t */
    seq_t s = floor_sqrt(t);

    /* a(n) = s - k */
    return s - k;
}
