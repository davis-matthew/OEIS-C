/*
 * A066479  a(n) = min( x : x^3+n^3+x^2+n^2+x+n=1 mod(x+n)).
 * https://oeis.org/A066479
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ d-n : d divides 2*n*n-1 and d > n }
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 56 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    if (n < 2) return 0;          /* defined only for n ≥ 2 */

    seq_t sn = (seq_t)n;
    seq_t m  = sn * sn * (seq_t)2 - (seq_t)1;   /* 2*n^2 - 1 */

    seq_t best = m;                     /* the largest possible divisor */

    seq_t lim   = floor_sqrt(m);        /* floor( √m ) */
    idx_t limit = (idx_t)lim;           /* safe cast: lim ≤ 2^64-1 */

    for (idx_t i = 1; i <= limit; ++i)
    {
        seq_t di = (seq_t)i;
        if (m % di == (seq_t)0)
        {
            seq_t j = m / di;           /* complementary divisor */

            if (di > sn && di < best) best = di;
            if (j  > sn && j  < best) best = j;
        }
    }

    return best - sn;                   /* a(n) = d_min - n */
}
