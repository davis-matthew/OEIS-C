/*
 * A043208  Numbers k such that 4 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_4_or_7_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043208: numbers k such that the base‑9 representation of k
   contains the digits 4 and 7 next to each other (either order),
   whereas k‑1 does not have this property.  The function returns
   a(n) for a(1) = 43.  It is implemented by a lazy scan of successive
   integers and caching the results, allowing calls in arbitrary order. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_4_or_7_in_base9.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    /* Static cache: indices of qualifying k values */
    static idx_t *cache_idx   = NULL; /* array of stored k values   */
    static size_t cached_len  = 0;    /* number of filled entries    */
    static size_t cache_cap   = 0;    /* allocated capacity          */
    static idx_t  last_k      = 0;    /* last k that was processed   */
    static bool    last_has47 = false;/* whether last_k had adjacency*/

    /* Grow the cache if we need more terms */
    if (cached_len < (size_t)n) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < needed) new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(cache_idx, new_cap * sizeof(idx_t));
            if (!tmp) return (seq_t)0; /* allocation failed – unlikely */
            cache_idx = tmp;
            cache_cap = new_cap;
        }

        /* Continue scanning integers k until we have enough terms */
        for (idx_t k = last_k + 1; cached_len < (size_t)n; ++k) {
            bool has_adj = has_adjacent_4_or_7_in_base9(k);
            if (has_adj && !last_has47)
                cache_idx[cached_len++] = k;
            last_has47 = has_adj;
            last_k = k;
        }
    }

    /* Return the cached value, converting to seq_t */
    return (seq_t)cache_idx[n - 1];
}
