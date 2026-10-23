/*
 * A142131  Primes congruent to 22 mod 37.
 * https://oeis.org/A142131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 187, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≡ 22 (mod 37)  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Static cache of computed terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next k such that the candidate 22 + 37*k is tested */
static uint64_t current_k = 1;

/* Grow the cache array when it runs out of space */
static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache     = tmp;
    cache_cap = new_cap;
}

/* Ensure that at least n terms are available in the cache */
static void ensure_n(idx_t n) {
    if (cache_len >= n) return;                /* already have enough */

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Search for further primes of the form 22 + 37*k. */
    while (cache_len < n) {
        uint64_t candidate = 22 + 37 * current_k;
        if (is_prime_small(candidate)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)candidate;
        }
        current_k++;
    }
}

/* Return the n-th prime ≡ 22 (mod 37).  a(1) = 59. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1; placeholder */
    ensure_n(n);
    return cache[(size_t)n - 1];
}
