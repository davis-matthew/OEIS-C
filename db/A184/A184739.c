/*
 * A184739  floor(n*s+h-h*s), where s=3+sqrt(5), h=1/2; complement of A184738.
 * https://oeis.org/A184739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(3+sqrt(5)) + 1/2 - (1/2)*(3+sqrt(5)))
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
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
    /* Return 0 for n == 0 (out of formal range). */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t n_s = (seq_t)n;
    seq_t m   = (seq_t)2 * n_s - (seq_t)1;           /* m = 2n – 1          */
    seq_t t   = (seq_t)5 * m * m;                   /* t = 5 * m²           */
    seq_t s   = floor_sqrt(t);                      /* floor(sqrt(t)) = floor(m*sqrt(5)) */
    seq_t h   = s / (seq_t)2;                       /* floor(m*sqrt(5)/2)   */
    seq_t res = (seq_t)3 * n_s - (seq_t)1 + h;      /* 3n – 1 + floor(m*sqrt(5)/2) */
    return res;
}
