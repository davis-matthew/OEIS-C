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
 * Helpers:   floor_sqrt
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
#include "../utils/floor_sqrt.h"

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
