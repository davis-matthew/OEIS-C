/*
 * A142396  Primes congruent to 45 mod 47.
 * https://oeis.org/A142396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 45 mod 47
 * Range:     u8 n <= 2, u16 n <= 150, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;
    /* Current candidate to test (next value of the form 45 + k*47) */
    static idx_t last_candidate = 45;

    /* Return cached value if available */
    if (n <= cache_size)
        return cache[n - 1];

    /* Ensure the cache is allocated and has sufficient capacity */
    if (!cache)
    {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n)
    {
        /* Test the current candidate for primality */
        if (is_prime_small((seq_t)last_candidate))
        {
            /* Grow cache if needed */
            if (cache_size == cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            /* Store the found prime */
            cache[cache_size++] = (seq_t)last_candidate;
        }
        /* Move to the next number of the form 45 + k*47 */
        last_candidate += 47;
    }

    return cache[n - 1];
}
