/*
 * A190330  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(2), t=1/s.
 * https://oeis.org/A190330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n / sqrt(2)) + floor(n / 2)
 * Range:     u8 n <= 5, u16 n <= 76, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9309 (2026-11-01)
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
    seq_t nn = (seq_t)n;                     /* cast index to sequence type */
    /* floor(n / sqrt(2)) = floor( n*sqrt(2)/2 )
     * compute floor( sqrt(2*n*n) / 2 ) to avoid floating point */
    seq_t sqrt_val = floor_sqrt(((seq_t)2) * nn * nn);
    seq_t floor_n_sqrt2 = sqrt_val / 2;      /* integer division gives floor */

    return nn + floor_n_sqrt2 + (nn / 2);    /* n + [n/sqrt2] + [n/2] */
}
