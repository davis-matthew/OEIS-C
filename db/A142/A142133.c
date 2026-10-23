/*
 * A142133  Primes congruent to 24 mod 37.
 * https://oeis.org/A142133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 148, u32 n <= 154, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 24 (mod 37) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached primes ≡24 (mod 37) */
static size_t cache_len = 0;         /* number of elements cached */
static size_t cache_cap = 0;         /* allocated size of cache */
static uint64_t next_candidate = 24; /* last tested candidate */

/* Ensure the cache contains at least n_needed elements */
static void ensure_cache(idx_t n_needed)
{
    if (cache_len >= (size_t)n_needed)
        return;

    /* Initialise cache if first use */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_len < (size_t)n_needed) {
        next_candidate += 37;                 /* next number ≡24 (mod 37) */
        if (is_prime_big((seq_t)next_candidate)) {
            if (cache_len >= cache_cap) {     /* grow cache if full */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache,
                                          sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset = 1: a(1) is the first such prime */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
