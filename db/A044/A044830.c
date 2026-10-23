/*
 * A044830  Positive integers having more base-4 runs of even length than odd.
 * https://oeis.org/A044830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   even_runs_more
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044830: a(n) is the n‑th positive integer that has more
   base‑4 runs of even length than runs of odd length.  The function
   searches successive integers and keeps those that satisfy the
   condition. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/even_runs_more.h"

/* Return a(n), the n‑th positive integer with the property above.  The
   function keeps an internal cache so that calls in any order remain
   efficient. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 1;

    /* Extend the cache until we have at least n terms. */
    while (cache_used < (size_t)n) {
        if (even_runs_more(next_candidate)) {
            if (cache_used == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    cache_used = 0;
                    cache_cap = 0;
                    return (seq_t)0;   /* allocation failure */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_used++] = next_candidate;
        }
        next_candidate++;
        /* If next_candidate overflows seq_t, wrap to 1 to avoid an
           infinite loop.  For typical tests this does not occur. */
        if (next_candidate == (seq_t)0)
            next_candidate = (seq_t)1;
    }
    return cache[(size_t)n - 1];
}
