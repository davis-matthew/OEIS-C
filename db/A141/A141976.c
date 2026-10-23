/*
 * A141976  Primes congruent to 27 mod 28.
 * https://oeis.org/A141976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 27 modulo 28
 * Range:     u8 n <= 5, u16 n <= 556, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes on demand and caches them for later calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* cached primes = a(1), a(2), ...          */
static size_t cache_cap = 0;         /* capacity of cache                         */
static idx_t next_candidate = 27;    /* next number to test (will be increased by 28) */
static size_t cache_len = 0;         /* number of elements stored in cache         */

/* Ensure the cache contains at least n primes of the required form. */
static void ensure_cache(idx_t n)
{
    /* Lazy allocator for cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate primes until we have enough */
    while ((idx_t)cache_len < n) {
        /* Move to the next candidate congruent to 27 mod 28 */
        next_candidate += 28;
        if (is_prime(next_candidate)) {
            /* Resize cache if needed */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = (seq_t)next_candidate;
            cache_len++;
        }
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Calls with n < 1 are undefined. */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
