/*
 * A142101  Primes congruent to 5 mod 36.
 * https://oeis.org/A142101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 5 modulo 36
 * Range:     u8 n <= 4, u16 n <= 548, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* array of found primes */
static idx_t  cached_count = 0;  /* how many primes are stored */
static size_t cache_cap    = 0;  /* current capacity of cache */
static idx_t  next_candidate = 5; /* next number ≡5 (mod 36) to test */

/* Ensure that cache contains at least n primes.  */
static void ensure_cache(idx_t n)
{
    if (cached_count >= n)
        return;

    /* Allocate or grow the cache as needed. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = seq_realloc(NULL, 0, cache_cap);
    }

    while (cached_count < n) {
        if (is_prime(next_candidate)) {
            if (cached_count >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cached_count++] = (seq_t)next_candidate;
        }
        next_candidate += 36; /* keep the congruence 5 (mod 36) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1; 0 is unused */
    ensure_cache(n);
    return cache[n - 1];
}
