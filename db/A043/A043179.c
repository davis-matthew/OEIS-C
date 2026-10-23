/*
 * A043179  Numbers k such that 6 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_6_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers k such that the base‑8 representation of k contains
   the digits 6 and 7 adjacent (either order) but the base‑8 representation
   of k−1 does NOT contain such adjacency.  The function searches
   sequentially, caching found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_6_7.h"

/* Return the n‑th term of the sequence (a(1) = 55). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined for n=0 */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t next_k = (seq_t)1;              /* next integer to test */

    /* Allocate an initial capacity if needed. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (has_adjacent_6_7(next_k) && !has_adjacent_6_7(next_k - (seq_t)1)) {
            /* Grow cache if full. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_len++] = next_k;
        }
        next_k = next_k + (seq_t)1;
    }

    return cache[n - 1];
}
