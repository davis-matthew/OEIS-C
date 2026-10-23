/*
 * A128926  Smaller member p of a pair of consecutive primes (p,q) such that
 *          either q^2-p^2+1 or q^2-p^2-1 is also prime.
 * https://oeis.org/A128926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smaller prime p of a consecutive prime pair (p,q) such
 *            that * either q^2−p^2+1 or q^2−p^2−1 is also prime. *
 *            Computation: sequentially generate consecutive primes and test
 *            the condition, * caching results for arbitrary order of calls
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

/* Find the next prime greater than n using an inexpensive primality test. */
static uint64_t next_prime_uint64(uint64_t n)
{
    uint64_t cand = n + 1;
    while (!is_prime_small((seq_t)cand))
        ++cand;
    return cand;
}

seq_t a(idx_t n)
{
    /* Static state: cache of found terms, prime list pointer, and previous prime. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t prev_prime = 2;   /* start with 2, so first pair is (2,3) */

    /* Initialise the cache once. */
    if (!cache)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        uint64_t cur_prime = next_prime_uint64(prev_prime);

        /* Current pair (p,q). */
        seq_t p = (seq_t)prev_prime;
        seq_t q = (seq_t)cur_prime;

        /* Compute q^2 - p^2. */
        seq_t diff = q * q;
        diff -= p * p;          /* diff = q^2 - p^2, always positive */

        seq_t val1 = diff + (seq_t)1;
        seq_t val2 = diff - (seq_t)1;

        /* If either candidate is prime, store p. */
        if (is_prime_big(val1) || is_prime_big(val2))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* Basic error check – abort if allocation fails. */
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }

        prev_prime = cur_prime;
    }

    return cache[n - 1];
}
