/*
 * A101630  Denominator of partial sums of a certain series.
 * https://oeis.org/A101630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(15*sum_{k=1}^n 1/((4k-1)k(4k+1)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   gcd128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd128.h"

/* Return the n-th term of OEIS A101630: the denominator of the
   partial sums.  
   seq_t – integer type supplied by the build (> 64 bits may be an
   arbitrary-precision class).  idx_t – usually uint64_t. */
seq_t a(idx_t n)
{
    /* maintain the sum in lowest terms: num/den */
    unsigned __int128 num  = 0;   /* numerator, may grow very large */
    unsigned __int128 den  = 1;   /* denominator, reduced at each step */

    for (idx_t k = 1; k <= n; ++k) {
        /* term t_k = 15 / ((4k-1) * k * (4k+1)) */
        unsigned __int128 kk   = (unsigned __int128)k;
        unsigned __int128 t1   = 4*kk - 1;        /* 4k-1  */
        unsigned __int128 t3   = 4*kk + 1;        /* 4k+1  */
        unsigned __int128 tden = t1 * kk * t3;    /* denominator of term */
        unsigned __int128 tnum = 15;              /* numerator of term */

        /* reduce the term itself */
        unsigned __int128 g_t = gcd128(tnum, tden);
        tnum /= g_t;
        tden /= g_t;

        /* add t_k to current sum: (num/den) + (tnum/tden) */
        unsigned __int128 g1 = gcd128(den, tden);          /* gcd of denominators */
        unsigned __int128 lcm_part = tden / g1;            /* part for lcm */
        unsigned __int128 new_den = den * lcm_part;        /* new denominator */

        unsigned __int128 new_num = num * lcm_part
                                 + tnum * (den / g1);     /* new numerator */

        /* reduce the resulting fraction */
        unsigned __int128 g2 = gcd128(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    /* den now holds the reduced denominator of S_n */
    return (seq_t)den;
}
