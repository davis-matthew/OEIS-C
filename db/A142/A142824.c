/*
 * A142824  Primes congruent to 26 mod 61.
 * https://oeis.org/A142824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime that is congruent to 26 modulo 61. */
/* The implementation simply scans numbers of the form 26 + 61*k, tests
 * each for primality using the provided is_prime_small routine, and
 * caches all found primes to answer future queries quickly. */

#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Cached sequence of primes ≡ 26 (mod 61) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0)          /* undefined for offset 1; return 0 as a sentinel */
        return (seq_t)0;

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Start searching from the last stored prime + 61, or from 26 if empty */
    seq_t candidate;
    size_t count = cache_len;
    if (count == 0) {
        candidate = (seq_t)26;
    } else {
        candidate = cache[cache_len - 1] + (seq_t)61;
    }

    /* Extend cache until we have computed the n-th term */
    while (cache_len < (size_t)n) {
        if (is_prime_small(candidate)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap * 2 + 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
        candidate += (seq_t)61;
    }

    return cache[(size_t)n - 1];
}
