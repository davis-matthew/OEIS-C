/*
 * A074599  Numerator of 2 * H(n,2,1), a generalized harmonic number. See
 *          A075135. Also 2 * A350669.
 * https://oeis.org/A074599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator( 2 * sum_{k=0}^{n-1} 1/(2k+1) ) (i.e. numerator
 *            of 2 * H_{odd,n} after reduction)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator of the fraction */
    seq_t den = 1;   /* current denominator of the fraction */

    for (idx_t k = 0; k < n; ++k) {
        seq_t term_num = 2;                     /* 2/(2k+1) numerator */
        seq_t term_den = (idx_t)(2*k + 1);      /* denominator of the current term */

        /* Reduce the addition: compute lcm of current denominator and term denominator. */
        seq_t g = gcd(den, term_den);           /* gcd of denominators */
        seq_t lcm_den = (den / g) * term_den;   /* lcm = den * term_den / g */

        /* New numerator after adding the term. */
        seq_t tmp1 = num * (lcm_den / den);
        seq_t tmp2 = term_num * (lcm_den / term_den);
        seq_t new_num = tmp1 + tmp2;
        seq_t new_den = lcm_den;

        /* Reduce the fraction again to keep numbers manageable. */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* numerator of the reduced fraction 2*H_{odd,n} */
}
