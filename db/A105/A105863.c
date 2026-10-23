/*
 * A105863  a(n) = n * Sum_{d|n} (binomial(n,d) / gcd(n,d)).
 * https://oeis.org/A105863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} binomial(n,d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* just in case */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)                  /* d is a divisor of n */
        {
            sum += binomial(n, d);       /* add term for divisor d */
            idx_t e = n / d;             /* complementary divisor */
            if (e != d)
                sum += binomial(n, e);   /* add term for e if distinct */
        }
    }
    return (seq_t)n * sum;
}
