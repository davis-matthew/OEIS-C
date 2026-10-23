/*
 * A043238  Numbers k such that 2 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains28_or82
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043238:
   numbers k such that the decimal digits 2 and 8 occur next to each other
   in k (in either order) but not in k-1. */
/* No explicit formula; function searches numbers incrementally and
   caches results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains28_or82.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, return 0 as a safe value */
        return (seq_t)0;

    /* Persistent storage for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = (seq_t)27;   /* start just before first term */

    /* Expand the cache until we have computed at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k = last_checked + (seq_t)1;   /* next candidate value */

        if (contains28_or82(k) && !contains28_or82(k - (seq_t)1)) {
            /* Ensure allocator capacity */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache)          /* memory allocation failure */
                    return (seq_t)0;
            }
            cache[cache_len++] = k;
        }

        last_checked = k;
    }

    return cache[(size_t)n - 1];
}
