/*
 * A009205  a(n) = gcd(d(n), sigma(n)).
 * https://oeis.org/A009205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(d(n), sigma(n)) where d(n) = number of divisors of n
 *            and sigma(n) = sum of divisors of n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; undefined below */

    seq_t sigma = 0;     /* sum of divisors */
    idx_t d = 0;         /* count of divisors */

    for (idx_t i = 1; (seq_t)i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            seq_t div1 = (seq_t)i;
            seq_t div2 = (seq_t)(n / i);

            sigma += div1;
            ++d;

            if (div1 != div2)
            {
                sigma += div2;
                ++d;
            }
        }
    }

    return seq_gcd((seq_t)d, sigma);
}
