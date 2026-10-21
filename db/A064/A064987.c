/*
 * A064987  a(n) = n*sigma(n).
 * https://oeis.org/A064987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n * sigma(n) (sigma = sum of divisors)
 * Range:     u8 n <= 11, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute sigma(n) */
    uint64_t nn = (uint64_t)n;
    seq_t sigma = 1;
    uint64_t temp = nn;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t term = 1;      /* sum of powers of p */
            uint64_t power = 1;     /* p^k */

            while (temp % p == 0)
            {
                temp /= p;
                power *= p;
                term += power;
            }
            sigma *= term;
        }
    }

    if (temp > 1) /* remaining prime factor */
        sigma *= (1 + temp);

    return (seq_t)n * sigma;
}
