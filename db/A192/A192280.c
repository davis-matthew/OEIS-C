/*
 * A192280  Characteristic function of numbers that are the product of
 *          consecutive primes.
 * https://oeis.org/A192280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a product of consecutive primes (including *
 *            any single prime), otherwise 0. Implementation uses trial
 *            division * and checks that prime factors are exactly consecutive
 *            in the prime * sequence
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the next prime greater than x. */
static uint64_t next_prime_after(uint64_t x)
{
    uint64_t y = x + 1;
    while (!is_prime(y))
        ++y;
    return y;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* a(1) is defined as 1 */

    uint64_t m = n;                    /* number to factor */
    uint64_t prev_prime = 0;           /* previous prime factor */

    /* trial division over all primes up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            /* check exponent; must be exactly 1 */
            unsigned cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
                if (cnt > 1)
                    return (seq_t)0;   /* not square‑free */
            }

            /* check that current prime follows the previous one consecutively */
            if (prev_prime)
            {
                if (p != next_prime_after(prev_prime))
                    return (seq_t)0;   /* gap in the prime sequence */
            }
            prev_prime = p;
        }
    }

    /* any remaining prime factor larger than sqrt(original n) */
    if (m > 1)
    {
        uint64_t p = m;                /* exponent is 1 automatically */
        if (prev_prime)
        {
            if (p != next_prime_after(prev_prime))
                return (seq_t)0;       /* missing intermediate prime */
        }
    }

    return (seq_t)1;
}
