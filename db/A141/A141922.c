/*
 * A141922  Primes congruent to 18 mod 23.
 * https://oeis.org/A141922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 298, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Sequence: primes congruent to 18 mod 23
   a(n) is the n‑th prime p such that p ≡ 18 (mod 23), n ≥ 1. */

/* The function builds the sequence lazily and memoises it in a
   static array so subsequent calls with arbitrary n are O(1).  The
   array is expanded in powers of two.  The index argument n is
   expected to be ≥ 1; for n ≤ 0 the function returns 0. */

static seq_t *cache = NULL;       /* array of found primes */
static size_t cache_len = 0;      /* number of entries in cache      */
static size_t cache_cap = 0;      /* allocated size of cache         */
static idx_t next_k = 0;          /* next k to test (p = 18 + 23*k) */
static bool initialized = false; /* one‑time initialisation flag     */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe return */

    if (!initialized) {
        initialized = true;
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        next_k = 0;
    }

    /* Generate more primes until we have at least n terms */
    while (cache_len < n) {
        /* candidate = 18 + 23*k ;  k starts at 0 */
        idx_t cand64 = 18 + 23 * next_k;
        seq_t candidate = (seq_t)cand64;

        if (is_prime_small(candidate)) {
            /* Ensure enough capacity for the new element */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – stop gracefully */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}
