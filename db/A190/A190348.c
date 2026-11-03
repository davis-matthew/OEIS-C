/*
 * A190348  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(5/3), t=sqrt(3/5).
 * https://oeis.org/A190348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3/5)) + floor(3*n/5)
 * Range:     u8 n <= 10, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9224 (2026-11-01)
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
    /* Intermediate arithmetic is done in idx_t to avoid overflow for
       large n; the final value is then converted to seq_t. */
    idx_t nn = n;
    idx_t t3  = (nn * 3) / 5;          /* floor(3*n/5)        */
    idx_t tri = (nn * nn * 3) / 5;     /* floor((3*n^2)/5)   */

    seq_t sroot = floor_sqrt((seq_t)tri);  /* floor(n*sqrt(3/5)) */
    seq_t sn     = (seq_t)nn;
    seq_t s_t3   = (seq_t)t3;

    return sn + sroot + s_t3;
}
