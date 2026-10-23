/*
 * A039701  a(n) = n-th prime modulo 3.
 * https://oeis.org/A039701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) % 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static storage for the primes that have already been generated. */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t  prime_limit = 0;

    /* The sequence is defined only for n >= 1. */
    if (n == 0)
        return 0; /* offset not defined – return 0 */

    /* Do we already have at least n primes? */
    if (prime_count < (size_t)n)
    {
        /* Compute a safe upper bound for the nth prime. */
        idx_t needed_limit = _prime_limit_for_idx(n);

        /* Re‑build the prime table if the current one is insufficient. */
        if (prime_limit < needed_limit)
        {
            free(primes);
            primes = NULL;
            prime_limit = needed_limit;

            build_primes_upto(prime_limit, &primes, &prime_count);

            /* The routine is guaranteed to return at least one prime per
               integer increment of limit, but if for any reason the count
               is still below n we keep doubling the limit until it is. */
            while (prime_count < (size_t)n)
            {
                prime_limit *= 2;
                free(primes);
                build_primes_upto(prime_limit, &primes, &prime_count);
            }
        }
    }

    /* Retrieve the nth prime. */
    uint64_t p = primes[(size_t)n - 1];
    return (seq_t)(p % 3);
}
