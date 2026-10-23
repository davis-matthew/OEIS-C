/*
 * A048315  Numbers whose base-4 expansions, read from left to right, have run
 *          lengths that strictly decrease.
 * https://oeis.org/A048315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑4 representation has run
 *            lengths that strictly decrease from left to right
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   strict_decreasing_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/strict_decreasing_base4.h"

/* Static cache for the sequence values.  The cache is expanded only as
   needed, so repeated calls to `a(n)` work efficiently. */
static seq_t *cache      = NULL;   /* array of stored terms          */
static size_t cache_cap  = 0;      /* allocated capacity             */
static size_t cache_size = 0;      /* number of terms stored         */
static idx_t  next_candidate = 0;  /* next integer to test for sequence */

/* Expand the cache until it contains at least `target_index+1` terms. */
static void extend_cache(idx_t target_index) {
    while (cache_size <= target_index) {
        /* test the current candidate */
        if (strict_decreasing_base4(next_candidate)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out‑of‑memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n) {
    if (cache_size <= n)
        extend_cache(n);      /* fill cache up to the requested index */
    return cache[n];          /* return the required term */
}
