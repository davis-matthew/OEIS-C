/*
 * A017682  Denominator of sum of -9th powers of divisors of n.
 * https://oeis.org/A017682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^9 / gcd( sigma_9(n), n^9 ) where sigma_9(n) = sum_{d|n}
 *            d^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute n^9 */
    seq_t sn = (seq_t)n;
    seq_t n9 = 1;
    for (int i = 0; i < 9; ++i) n9 *= sn;

    /* compute sigma_9(n) */
    seq_t sigma9 = 1;
    idx_t tmp = n;

    /* trial division up to sqrt(tmp) */
    for (idx_t d = 2; d * d <= tmp; d += (d == 2 ? 1 : 2))
    {
        if (tmp % d == 0)
        {
            idx_t e = 0;
            while (tmp % d == 0) { tmp /= d; ++e; }

            /* compute sum_{i=0..e} (d^9)^i */
            seq_t p9 = 1;
            for (int i = 0; i < 9; ++i) p9 *= (seq_t)d;

            seq_t term = 1;
            seq_t sum_p = 1;   /* i = 0 term */
            for (idx_t i = 1; i <= e; ++i)
            {
                term *= p9;      /* (d^9)^i */
                sum_p += term;
            }
            sigma9 *= sum_p;
        }
    }

    /* remaining prime factor > 1 */
    if (tmp > 1)
    {
        seq_t p9 = 1;
        for (int i = 0; i < 9; ++i) p9 *= (seq_t)tmp;

        seq_t term = p9;   /* e == 1 */
        seq_t sum_p = 1 + term;
        sigma9 *= sum_p;
    }

    /* denominator: n^9 / gcd(n9, sigma9) */
    seq_t g = gcd(n9, sigma9);
    seq_t denom = n9 / g;
    return denom;
}
