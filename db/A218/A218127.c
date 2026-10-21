/*
 * A218127  Number of 10-ary sequences with primitive period n.
 * https://oeis.org/A218127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 10^d * mu(n/d) for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mu_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* pow10(idx_t e) -> seq_t */
#include "../utils/mu_of.h"   /* int mu_of(idx_t n) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    /* iterate over divisors of n */
    for (idx_t d = (idx_t)1; d * d <= n; ++d)
    {
        if (n % d == (idx_t)0)
        {
            /* first divisor d */
            idx_t div1 = d;
            int mu1 = mu_of(n / div1);
            if (mu1 != 0)
            {
                seq_t term = pow10(div1);
                if (mu1 > 0)
                    sum += term;
                else            /* mu1 == -1 */
                    sum -= term;
            }

            /* paired divisor n/d */
            idx_t div2 = n / d;
            if (div2 != div1)
            {
                int mu2 = mu_of(n / div2);
                if (mu2 != 0)
                {
                    seq_t term = pow10(div2);
                    if (mu2 > 0)
                        sum += term;
                    else            /* mu2 == -1 */
                        sum -= term;
                }
            }
        }
    }

    return sum;
}
