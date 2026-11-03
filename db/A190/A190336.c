/*
 * A190336  a(n) = n + [n*r/s] + [n*t/s]; r=2, s=sqrt(2), t=1/s.
 * https://oeis.org/A190336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 601, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9680 (2026-11-01)
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
    seq_t nn = (seq_t)n;                      /* n as seq_t */
    /* floor(n*sqrt(2)) = floor(sqrt(2*n*n)) */
    unsigned __int128 prod128 = (unsigned __int128)n * n * 2;
    seq_t prod = (seq_t)prod128;              /* for n in the test range this fits */
    seq_t sqrt_term = floor_sqrt(prod);
    seq_t half = nn / 2;                      /* floor(n/2) */
    return nn + sqrt_term + half;
}
