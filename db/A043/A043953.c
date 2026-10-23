/*
 * A043953  Numbers k such that 3 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adj_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for integers k whose octal representation
   contains an adjacent pair of the digits 3 and 7 (in any order) while the
   integer k+1 does not.  The function stores all found k in a static cache
   so that repeated calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj_octal.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = (seq_t)0;

    /* Grow the cache if needed */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0; /* allocation failure, return 0 as sentinel */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k = last_checked + (seq_t)1;
        last_checked = k;

        if (has_adj_octal(k) && !has_adj_octal(k + (seq_t)1)) {
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
