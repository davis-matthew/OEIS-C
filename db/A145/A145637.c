/*
 * A145637  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=15.
 * https://oeis.org/A145637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∑_{d=1}^{2n} 15^{2n+1-d}/d, offset = 1
 *            A_ℓ(15) with ℓ = 2n+1 is computed as a rational number
 *            (numerator,denominator). The function returns the reduced
 *            numerator
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for size_t */
#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined below offset, safe return */

    /* ℓ = 2n+1 */
    idx_t l = 2 * n + 1;

    seq_t num = 0;           /* current numerator   */
    seq_t den = 1;           /* current denominator */

    /* build 15^(l-1) iteratively (cheap because 15 is small) */
    seq_t pow = 1;
    for (idx_t i = 0; i < l - 1; ++i)
        pow *= 15;          /* pow = 15^(l-1) */

    for (idx_t d = 1; d < l; ++d)            /* d ranges 1 .. l-1 = 2n */
    {
        seq_t term_num = pow;                 /* term numerator  */
        seq_t d_seq   = (seq_t)d;             /* convert d to seq_t */

        /* new fraction: num/den + term_num/d_seq */
        seq_t new_num = num * d_seq + term_num * den;
        seq_t new_den = den * d_seq;

        /* reduce */
        seq_t g = gcd_seq(new_num, new_den);
        if (g > 1) {
            new_num /= g;
            new_den /= g;
        }

        num = new_num;
        den = new_den;

        /* prepare next term exponent (divide by 15) */
        pow /= 15;
    }

    return num;   /* reduced numerator of A_l(15) */
}
