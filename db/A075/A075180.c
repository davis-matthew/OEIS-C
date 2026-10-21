/*
 * A075180  Denominators from e.g.f. 1/(1-exp(-x)) - 1/x.
 * https://oeis.org/A075180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(0)=2; a(n)=1 for even n>0, for odd n the denominator is *
 *            a(n)=∏_{p prime, p-1 | n+1} p^{1+v_p((n+1)/(p-1))}
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 70, u64 n <= 418,
 *            u128 n <= 2518, big any n
 * Helpers:   is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the helper functions from the utils library. */

#include "../utils/is_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* special case */
        return 2;
    if ((n & 1) == 0)      /* even n > 0 */
        return 1;

    idx_t m = n + 1;        /* m = n+1, which is even */
    seq_t result = 1;

    /* iterate over all divisors d of m */
    idx_t d, p, div;
    for (idx_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            /* first divisor */
            d = i;
            p = d + 1;
            if (is_prime(p))
            {
                div = m / d;          /* div = m/(p-1) */
                idx_t e = 0;
                while (div % p == 0)
                {
                    div /= p;
                    ++e;
                }
                result *= pow_uint(p, e + 1);
            }

            /* complementary divisor, skip if identical to avoid double counting */
            d = m / i;
            if (d != i)
            {
                p = d + 1;
                if (is_prime(p))
                {
                    div = m / d;      /* div = m/(p-1) */
                    idx_t e = 0;
                    while (div % p == 0)
                    {
                        div /= p;
                        ++e;
                    }
                    result *= pow_uint(p, e + 1);
                }
            }
        }
    }

    return result;
}
