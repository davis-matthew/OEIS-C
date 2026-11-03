/*
 * A194116  a(n) = Sum_{j=1..n} floor(j*sqrt(13)); n-th partial sum of Beatty
 *          sequence for sqrt(13).
 * https://oeis.org/A194116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*sqrt(13))
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t t = (seq_t)j * (seq_t)j;   /* j^2 */
        t *= (seq_t)13;                  /* 13 * j^2 */
        sum += floor_sqrt(t);            /* floor(sqrt(13*j^2)) = floor(j*sqrt(13)) */
    }
    return sum;
}
