/*
 * A217946  a(n) = 4^n*(n+1)*(8*n^2+32*n+33)*P(3/2,n)/(3*P(4,n)) where P(a,n)
 *          is the Pochhammer rising factorial.
 * https://oeis.org/A217946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217946
   a(n) = 4^n*(n+1)*(8*n^2+32*n+33)*P(3/2,n)/(3*P(4,n))
   Recurrence: a(0)=11
   (n+1)^2*(n+4)*(8*n^2+32*n+33)*a(n+1) =
        2*(2*n+3)*(n+2)*(8*n^2+48*n+73)*a(n)
*/
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t res = 11;
    for (idx_t k = 0; k < n; ++k)
    {
        /* Build numerator and denominator of the recurrence factor */
        seq_t num = 2;
        num *= (seq_t)(2 * k + 3);
        num *= (seq_t)(k + 2);
        num *= (seq_t)(8 * k * k + 48 * k + 73);

        seq_t den = (seq_t)(k + 1);
        den *= (seq_t)(8 * k * k + 32 * k + 33);
        den *= (seq_t)(k + 4);

        /* Cancel common factors to keep intermediate values small */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;

        /* Update the term */
        res = res * num / den;
    }
    return res;
}
