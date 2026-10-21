/*
 * A054719  Number of 4-ary sequences with primitive period n.
 * https://oeis.org/A054719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d)*4^(n/d)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   mu_of, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            int mu1 = mu_of(i);
            if (mu1)
            {
                seq_t p = pow_uint(4ULL, n / i);
                if (mu1 > 0)
                    sum += p;
                else
                    sum -= p;
            }

            /* paired divisor j = n/i if distinct */
            idx_t j = n / i;
            if (j != i)
            {
                int mu2 = mu_of(j);
                if (mu2)
                {
                    seq_t p = pow_uint(4ULL, n / j);
                    if (mu2 > 0)
                        sum += p;
                    else
                        sum -= p;
                }
            }
        }
    }
    return sum;
}
