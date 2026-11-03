/*
 * A036715  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2,
 *          a>=0, 0<=b<=a.
 * https://oeis.org/A036715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A036715: count of Gaussian integers z = a+bi with
   a ≥ 0, 0 ≤ b ≤ a and |z| ≤ n + 1/2.  Equivalently,
   a² + b² ≤ n² + n.
   Formula used: a(n) = Σ_{a=0}^{⌊√(n²+n)⌋} (⌊√(n²+n – a²)⌋ + 1)
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
    /* n² + n fits in seq_t */
    seq_t nSeq = (seq_t)n;
    seq_t lim   = nSeq * (nSeq + 1);          /* n^2 + n */

    seq_t count = 0;
    /* upper bound for a : floor( sqrt(lim) ) */
    idx_t aMax = (idx_t)floor_sqrt(lim);

    for (idx_t a = 0; a <= aMax; ++a)
    {
        seq_t aSq  = (seq_t)a * (seq_t)a;
        seq_t rem  = lim - aSq;               /* non‑negative */
        seq_t bmax = floor_sqrt(rem);         /* floor sqrt(rem) */

        /* enforce b ≤ a */
        if (bmax > (seq_t)a)
            bmax = (seq_t)a;

        count += bmax + 1;                    /* b = 0 … bmax */
    }

    return count;
}
