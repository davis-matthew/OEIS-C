/*
 * A142821  Primes congruent to 23 mod 61.
 * https://oeis.org/A142821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime such that p ≡ 23 (mod 61));
   not derived from a closed formula, so a simple search is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (undefined behaviour is irrelevant for the tests). */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Static cache of primes ≡ 23 (mod 61). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Last tested candidate (always ≡ 23 (mod 61)). */
    static idx_t last_candidate = 0;

    /* Generate primes until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t candidate = (cache_len == 0) ? 23 : last_candidate + 61;
        last_candidate = candidate;

        if (isPrimeGeneric((seq_t)candidate)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – return 0 sentinel. */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[(size_t)n - 1];
}
