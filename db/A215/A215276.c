/*
 * A215276  Primes congruent to {2, 4, 5, 6} mod 13.
 * https://oeis.org/A215276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 2, 4, 5 or 6 modulo 13 by searching primes */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* sequence values */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* current capacity of cache */

static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* Grow the cache allocation to make room for at least one more term. */
static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Ensure that cache has at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find next prime ≥ next_candidate. */
        uint64_t p = next_candidate;
        while (!is_prime(p)) ++p;
        next_candidate = p + 1;

        /* Check the required congruence. */
        uint64_t r = p % 13;
        if (r == 2 || r == 4 || r == 5 || r == 6) {
            grow_cache();
            cache[cache_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 safely */
    ensure_cache(n);
    return cache[n - 1];
}
