/*
 * A043458  Numbers having exactly two 1's in base 9.
 * https://oeis.org/A043458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest integer whose base‑9 representation
 *            contains exactly two digits equal to 1. (OEIS A043458)
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_ones_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_ones_base9.h"

/* Ensure the cache has room for at least one more element. */
static void grow_cache(size_t needed, seq_t **ptr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        if (new_ptr == NULL)
            abort();                      /* allocation failed */
        *ptr = new_ptr;
        *cap = new_cap;
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of discovered sequence values. */
    static seq_t *cache      = NULL; /* pointer to the cache array   */
    static size_t cache_len  = 0;    /* number of elements stored    */
    static size_t cache_cap  = 0;    /* allocated capacity           */
    static seq_t last_tested = (seq_t)0; /* last integer examined   */

    /* Return cached value if already computed. */
    if (n <= (idx_t)cache_len)
        return cache[(size_t)(n - 1)];

    /* Continue generating integers until the requested index is reached. */
    for (seq_t cand = last_tested + 1; ; ++cand) {
        if (count_ones_base9(cand) == 2) {
            /* Grow the cache if necessary. */
            grow_cache(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = cand;
            if (n <= (idx_t)cache_len) {
                last_tested = cand;
                return cand;
            }
        }
        last_tested = cand;
    }
}
