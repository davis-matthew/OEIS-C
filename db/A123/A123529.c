/*
 * A123529  Denominator of average of prime factors of n.
 * https://oeis.org/A123529
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Omega(n) / gcd( sum_of_prime_factors(n), Omega(n) )
 *            (Denominator of the reduced average of the prime factors of n)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at n = 2 */

    uint64_t temp = (uint64_t)n;
    seq_t sum     = 0;            /* sum of prime factors (with multiplicity) */
    seq_t Omega   = 0;            /* number of prime factors (with multiplicity) */

    /* trial division */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        while (temp % p == 0)
        {
            sum   += (seq_t)p;
            Omega += 1;
            temp  /= p;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1)
    {
        sum   += (seq_t)temp;
        Omega += 1;
    }

    /* average = sum / Omega; denominator of reduced fraction */
    seq_t g = gcd(sum, Omega);
    return Omega / g;
}
