/*
 * A142495  Primes congruent to 31 mod 51.
 * https://oeis.org/A142495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th prime congruent to 31 modulo 51
   (OEIS A142495). The function lazily searches
   successive candidates of the form 51*k+31 and caches
   the results so that later calls are fast. */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* next candidate to test (51*k + 31) */
    static idx_t candidate = 31;

    /* if N is less than or equal to the cache, return the cached value */
    if (n > 0 && cache_size >= (size_t)n)
        return cache[n - 1];

    /* otherwise keep searching until enough terms are found */
    while (cache_size < (size_t)n) {
        if (is_prime(candidate)) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                          (cache_size + 1) * sizeof(seq_t));
            if (!tmp) { /* allocation failed; abort gracefully */
                /* In an actual build, you might use abort() or similar. */
                return 0;
            }
            cache = tmp;
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate += 51;
    }

    return cache[n - 1];
}
