/*
 * A160717  Cyclops triangular numbers.
 * https://oeis.org/A160717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 1391, u64 n <= 8669,
 *            u128 n <= 8669, big any n
 * Helpers:   is_cyclops
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Cyclops triangular numbers:
   a(n) = the n‑th triangular number T_k that is also a cyclops number.
   (T_k = k*(k+1)/2, k >= 0; 0 is considered a cyclops number.) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_cyclops.h"

/* Return the n-th cyclops triangular number. */
seq_t a(idx_t n)
{
    /* Cache of previously found cyclops triangular numbers. */
    static seq_t *cache = NULL;     /* array of seq_t */
    static size_t cache_len = 0;    /* number of cached elements */
    static size_t cache_cap = 0;    /* capacity of the cache */

    /* Next triangular index to test (k), and the current triangular value. */
    static idx_t next_k = 0;        /* next k to evaluate (0,1,2,...) */
    static seq_t next_tri = 0;      /* T_next_k = k*(k+1)/2, computed incrementally */

    /* Grow the cache as needed. */
    if (cache_len >= (size_t)n)          /* already have the requested term */
        return cache[(size_t)n - 1];

    while (cache_len < (size_t)n) {       /* find as many terms as required */
        seq_t t = next_tri;              /* T_k */

        if (is_cyclops(t)) {
            /* Ensure sufficient capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Allocation failed – return 0 (undefined behaviour
                       in this unlikely case). */
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;
        }

        /* Advance to the next triangular number: T_{k+1} = T_k + (k+1). */
        ++next_k;
        next_tri += next_k;
    }

    return cache[(size_t)n - 1];
}
