/*
 * A045461  Primes congruent to {0, 1, 4} mod 7.
 * https://oeis.org/A045461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2168, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n‑th prime congruent to 0, 1, or 4 modulo 7.
   The sequence starts with 7, 11, 29, 43, …  (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but safe */

    /* Static memoisation of the primes found so far. */
    static seq_t *cache          = NULL;   /* array of primes */
    static size_t cache_cap      = 0;      /* allocated capacity */
    static size_t cache_count    = 0;      /* number of primes stored */
    static uint64_t next_candidate = 2;    /* next integer to test */

    /* Grow the cache until it contains at least n primes. */
    while (cache_count < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Candidates: 7 (exact), or residue 1 or 4 modulo 7. */
        if (cand == 7 || cand % 7 == 1 || cand % 7 == 4) {
            if (is_prime(cand)) {
                /* Ensure sufficient capacity. */
                if (cache_count >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp) {          /* allocation failure */
                        free(cache);
                        cache = NULL;
                        cache_cap = 0;
                        cache_count = 0;
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_count++] = (seq_t)cand;
            }
        }
        ++next_candidate;
    }

    /* Return the requested element (1‑based index). */
    return cache[n - 1];
}
