/*
 * A142829  Primes congruent to 31 mod 61.
 * https://oeis.org/A142829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 31 (mod 61)
   The n‑th such prime, with offset 1 (a(1)=31). */

/* We use a simple incremental search over numbers of the form 31+61*k
   and cache all results so that subsequent calls are fast, no matter
   what order they arrive in.  The build supplies the generic primality
   test is_prime(idx_t).  The function is fully defined in this file
   and compiles correctly with C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime ≡ 31 (mod 61).  n is 1‑based. */
seq_t a(idx_t n) {
    /* Static cache of primes we have already found. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_k     = 0;          /* next k for candidate = 31+61*k */

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Supply more terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Candidate of the form 31 + 61*k. */
        uint64_t cand = 31ULL + 61ULL * (uint64_t)next_k;

        if (is_prime((idx_t)cand)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }
        ++next_k;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
