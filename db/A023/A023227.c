/*
 * A023227  Primes p such that 7*p + 10 is also prime.
 * https://oeis.org/A023227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth prime p such that 7*p+10 is also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache to store found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* Last prime examined; start below the first prime to make next_prime work */
    static uint64_t last_checked = 2;

    /* a(1) is the first term; n must be >= 1 for valid calls.
       If n == 0 we return 0 as a placeholder. */
    if (n == 0)
        return 0;

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Generate additional primes until we have at least n elements */
    while (cache_size < (size_t)n) {
        uint64_t prime = next_prime(last_checked);
        last_checked = prime;

        /* Compute 7*prime + 10 and test its primality */
        uint64_t val = 7ull * prime + 10ull;
        if (is_prime(val)) {
            /* Ensure the cache has enough capacity and store the prime */
            ensure_cache(cache_size + 1, &cache, &cache_cap);
            cache[cache_size++] = (seq_t)prime;
        }
    }

    return cache[n - 1];
}
