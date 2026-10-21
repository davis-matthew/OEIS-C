/*
 * A069484  a(n) = prime(n+1)^2 + prime(n)^2.
 * https://oeis.org/A069484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 + prime(n+1)^2 (prime(1)=2, prime(2)=3, ...)
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 4791, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n<1, return 0 */

    /* static storage for generated primes */
    static uint32_t *primes = NULL;
    static size_t primes_len = 0;   /* number of primes stored */
    static size_t primes_cap = 0;   /* capacity of the array */

    /* generate primes until we have prime(n) and prime(n+1) */
    while (primes_len < (size_t)(n + 1))
    {
        /* start testing from last prime + 1 (or 2 for first prime) */
        uint32_t candidate = (primes_len == 0) ? 2U : primes[primes_len - 1] + 1U;

        /* search for the next prime */
        for (;; ++candidate)
        {
            int is_prime = 1;
            for (size_t i = 0; i < primes_len; ++i)
            {
                uint32_t p = primes[i];
                if ((uint64_t)p * p > candidate)
                    break;
                if (candidate % p == 0)
                {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime)
            {
                /* enlarge storage if necessary */
                if (primes_len + 1 > primes_cap)
                {
                    primes_cap = primes_cap ? primes_cap * 2 : 16;
                    primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                }
                primes[primes_len++] = candidate;
                break;
            }
        }
    }

    uint32_t prime_n   = primes[(size_t)(n - 1)]; /* prime(n) */
    uint32_t prime_np1 = primes[(size_t)n];      /* prime(n+1) */

    seq_t pn   = (seq_t)prime_n;
    seq_t pnp1 = (seq_t)prime_np1;

    return pn * pn + pnp1 * pnp1;
}
