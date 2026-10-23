/*
 * A137807  Final digit of prime(n)^2.
 * https://oeis.org/A137807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = last decimal digit of (prime(n))^2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of primes that survives across calls */
    static uint32_t *primes      = NULL;
    static size_t    primes_size = 0;
    static size_t    primes_cnt  = 0;
    static uint64_t  next_candidate = 2;    /* next number to test */

    if (n < 1)               /* n is 1‑based; undefined for n <= 0 */
        return (seq_t)0;

    /* Ensure we have at least n primes stored */
    size_t need = (size_t)n;
    if (primes_cnt < need)
    {
        /* Grow the storage if needed */
        if (primes_cnt == 0)
        {
            primes_size = 256;
            primes = (uint32_t *)realloc(NULL, primes_size * sizeof(uint32_t));
        }
        else if (primes_cnt == primes_size)
        {
            primes_size *= 2;
            primes = (uint32_t *)realloc(primes, primes_size * sizeof(uint32_t));
        }

        /* Generate primes until the required count is reached */
        while (primes_cnt < need)
        {
            uint64_t cand = next_candidate;
            int is_prime = 1;

            if (cand < 2)                 { is_prime = 0; }
            else if (cand == 2)           { is_prime = 1; }
            else if ((cand & 1ULL) == 0)  { is_prime = 0; }
            else
            {
                for (size_t i = 0; i < primes_cnt; ++i)
                {
                    uint32_t p = primes[i];
                    if ((uint64_t)p * p > cand) break;
                    if (cand % p == 0) { is_prime = 0; break; }
                }
            }

            if (is_prime)
                primes[primes_cnt++] = (uint32_t)cand;

            /* Prepare next candidate (skip even numbers after 2) */
            next_candidate = (cand == 2) ? 3 : cand + 2;
        }
    }

    /* Retrieve the n‑th prime (1‑based index) */
    uint32_t p  = primes[(size_t)(n - 1)];
    uint32_t d  = p % 10;
    uint32_t res = (d * d) % 10;

    return (seq_t)res;
}
