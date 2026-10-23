/*
 * A142391  Primes congruent to 40 mod 47.
 * https://oeis.org/A142391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 142, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th prime p with p ≡ 40 (mod 47) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Static cache of the sequence terms */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

/* Last candidate checked; starts from the smallest positive integer. */
static idx_t next_candidate = 2;

/* Ensure that cache has at least ‘min_cap’ elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) exit(1);          /* out of memory */
        cache = newp;
        cache_cap = new_cap;
    }
}

/* Extend the cache until it contains at least n terms. */
static void ensure_n(idx_t n)
{
    if (n == 0) return;              /* sequence is 1‑based */
    while (cache_len < (size_t)n) {
        /* Find the next prime ≡ 40 (mod 47) */
        while (!is_prime_big((seq_t)next_candidate) ||
               ((uint64_t)next_candidate % 47ULL) != 40) {
            next_candidate++;
        }
        /* Store the found prime */
        grow_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate++;           /* move past the found prime */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* no 0‑index in OEIS sequence */
    ensure_n(n);
    return cache[n - 1];             /* 1‑based indexing */
}
