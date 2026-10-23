/*
 * A142926  Primes congruent to 3 mod 64.
 * https://oeis.org/A142926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 195, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 3 (mod 64)
   a(1) = 3, a(2) = 67, a(3) = 131, …                              
   The function finds the nth prime of the form 64·k + 3 by testing
   consecutive candidates and caching the results.               */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Edge case: OEIS offset is 1; behaviour for n = 0 is irrelevant. */
    if (n == 0)
        return 0;

    /* Static cache of primes ≡ 3 (mod 64). */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    /* Next candidate of the form 64·k + 3 to test. */
    static idx_t candidate   = 3;

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure the cache can hold at least n elements. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory. */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Generate primes until we have the requested term. */
    while (cache_len < n) {
        if (is_prime(candidate))
            cache[cache_len++] = (seq_t)candidate;
        candidate += 64;              /* Next number ≡ 3 (mod 64). */
    }

    return cache[n - 1];
}
