/*
 * A155736  Numbers n such that 4*n^2+2*n-1 is a prime.
 * https://oeis.org/A155736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search over positive integers; keep those for which 4*n^2+2*n-1 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t n); */

seq_t a(idx_t n) {
    /* Static cache of already found terms and the next candidate to test. */
    static seq_t *cache = NULL;          /* array of found values */
    static size_t cache_cap = 0;        /* allocated size of cache */
    static size_t cache_len = 0;        /* number of terms stored */
    static idx_t next_candidate = 1;    /* candidate integer to test next */

    if (n == 0) return (seq_t)0;        /* undefined, but avoid UB */

    /* Grow cache incrementally until we have at least n terms. */
    while (cache_len < n) {
        /* Compute v = 4*next_candidate^2 + 2*next_candidate - 1 as seq_t. */
        seq_t cand = (seq_t)next_candidate;
        seq_t v = (seq_t)4 * cand * cand + (seq_t)2 * cand - (seq_t)1;

        /* If v is prime, store the candidate as a term. */
        if (is_prime_big(v)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed; abort gracefully */
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_len = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        ++next_candidate;  /* Move to the next integer to test */
    }

    return cache[n - 1];   /* sequence is 1-indexed */
}
