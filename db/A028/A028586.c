/*
 * A028586  Theta series of lattice with Gram matrix [2 1; 1 3].
 * https://oeis.org/A028586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7550, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A028586: number of integer solutions to 2*x^2 + 2*x*y + 3*y^2 = n.
   Implementation uses the equivalent condition u^2 + 5*y^2 = 2*n
   with u = 2*x + y, and parity constraint u ≡ y (mod 2).
   Complexity: O(√n) per call.
*/
#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Compute maximum |y|: 5*y^2 <= 2*n  =>  |y| <= sqrt(2*n/5) */
    seq_t limit_val = (seq_t)2 * N / (seq_t)5;
    idx_t Ymax = (idx_t)floor_sqrt(limit_val);

    seq_t count = 0;   /* result accumulator */

    for (int64_t y = -(int64_t)Ymax; y <= (int64_t)Ymax; ++y)
    {
        seq_t ysq = (seq_t)1 * (seq_t)y * (seq_t)y;   /* y*y */
        seq_t r   = (seq_t)2 * N - (seq_t)5 * ysq;   /* r = 2*n - 5*y^2, >=0 by construction */

        /* r must be a perfect square */
        seq_t sqrt_r = floor_sqrt(r);
        if (sqrt_r * sqrt_r != r)
            continue;

        /* parity check: u ≡ y (mod 2) */
        seq_t uparity = sqrt_r & (seq_t)1;
        seq_t yparity = (seq_t)y & (seq_t)1;

        if (sqrt_r == 0)
        {
            if (uparity == yparity)
                count += 1;   /* only u = 0
                                * satisfies parity when sqrt_r==0
                                */
        }
        else    /* sqrt_r > 0 => u = ±sqrt_r, both have same parity */
        {
            if (uparity == yparity)
                count += 2;
        }
    }
    return count;
}
