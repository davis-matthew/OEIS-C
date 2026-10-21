/*
 * A061833  Multiples of 11 having only odd digits.
 * https://oeis.org/A061833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer that is a multiple of 11 and
 *            has only odd decimal digits (1,3,5,7,9)
 * Range:     u8 n <= 5, u16 n <= 222, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   all_odd_digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/all_odd_digits_seq_t.h"

/* Return the n-th term of A061833 (n >= 1). */
seq_t a(idx_t n)
{
    /* Sequence starts at index 1. For any n < 1 return 0 as undefined. */
    if (n < 1) return (seq_t)0;

    /* Cache of already found terms (static prevents reallocation on each call). */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cache_size = 0;        /* number of cached terms */
    static size_t cache_cap = 0;         /* allocated capacity of cache */
    static seq_t multiplier = 1;         /* next multiplier to test (multiplied by 11) */

    /* Expand the cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        /* Compute candidate multiple of 11. */
        seq_t cand = ((seq_t)11) * multiplier;

        /* If the candidate consists only of odd digits, store it. */
        if (all_odd_digits_seq_t(cand)) {
            /* Grow the cache if necessary. */
            if (cache_cap == 0) {
                cache_cap = 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort(); /* allocation failure */
            } else if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        /* Advance to the next multiplier. */
        ++multiplier;
    }

    /* Return the requested term (indices are 1-based). */
    return cache[(size_t)n - 1];
}
