/*
 * A145639  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=16.
 * https://oeis.org/A145639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Σ_{d=1}^{2n} 16^(2n+1-d)/d, reduced to
 *            lowest terms
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* l is the odd index 2n+1 */
    idx_t l = 2 * n + 1;

    seq_t num = 0;   /* current numerator of the accumulated sum */
    seq_t den = 1;   /* current denominator of the accumulated sum */

    /* pow = 16^e, starting with e = 1 so that the first term
       is 16^(l-1)/1.  Each loop multiplies pow by 16 for the
       next term. */
    seq_t pow = 16;

    for (idx_t e = 1; e < l; ++e) {
        seq_t term_den = (seq_t)(l - e); /* l - e is the current d */
        seq_t term_num = pow;            /* 16^e */

        /* Add the fraction term_num/term_den to the accumulated
           fraction num/den.  The LCM method keeps intermediate
           numbers smaller. */
        seq_t g  = gcd(den, term_den);
        seq_t lcm = (den / g) * term_den;

        seq_t new_num = num * (lcm / den) + term_num * (lcm / term_den);
        seq_t new_den = lcm;

        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;

        /* Prepare the next power of 16 for the following term. */
        pow = pow * 16;
    }

    return num;
}
