/*
 * A145610  Denominator of the polynomial A_l(x) = Sum_{d=1..l-1} x^(l-d)/d
 *          for index l=2n+1 evaluated at x=1.
 * https://oeis.org/A145610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the reduced fraction of H_{2n} where H_m =
 *            1 + 1/2 + … + 1/m. The denominator of H_m is the denominator of
 *            the sum after reducing the fraction to lowest terms
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  The value for n = 0 is
       irrelevant for the OEIS entry but we return 0 to keep the
       routine robust.  */
    if (n == 0) return 0;

    idx_t m = n * 2;          /* m = 2n  */
    seq_t num = 0;            /* current numerator   */
    seq_t den = 1;            /* current denominator */

    for (idx_t d = 1; d <= m; ++d) {
        /* Add 1/d to the running fraction num/den.
           New fraction: (num*d + den) / (den*d )   */
        seq_t new_num = num * d + den;
        seq_t new_den = den * d;

        /* Reduce immediately to avoid massive intermediate sizes.  */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return den;
}
