/*
 * A044686  Numbers n such that string 6,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_60
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: Computes the nth term of OEIS A044686.
   A n such that the base‑9 representation contains the substring
   “60” but the base‑9 representation of n+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_60.h"

/* Cache of previously computed terms to speed repeated calls. */
static seq_t *cache      = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

/* Grow the cache to at least 'need' elements. */
static void ensure_cache(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Main function required by the specification. */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for the OEIS offset, but keep safe */

    ensure_cache(n);

    /* Grow cache to contain at least 'n' terms. */
    seq_t candidate = 1;
    if (cache_len) candidate = cache[cache_len - 1] + 1;   /* start after last cached */

    while (cache_len < (size_t)n) {
        if (has_60(candidate) && !has_60(candidate + 1)) {
            /* store term */
            cache[cache_len++] = candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
