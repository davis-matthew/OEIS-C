/*
 * A044027  Numbers k such that 4 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number k such that 4 and 6 occur juxtaposed in the
 *            base-10 representation of k but not in k+1
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_juxtaposed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence lazily in a static cache. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_juxtaposed.h"

/* Grow cache to accommodate at least need elements. */
static void grow_cache(size_t need, seq_t **arr, size_t *cap)
{
    if (need <= *cap)
        return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Main sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* behaviour below offset is irrelevant */

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Extend the cache if necessary. */
    while (cache_len < (size_t)n) {
        seq_t cur = 1;
        /* If cache not empty, start from last generated number + 1. */
        if (cache_len > 0)
            cur = cache[cache_len - 1] + 1;

        /* Scan forward until we find the next term. */
        while (true) {
            if (has_juxtaposed(cur) && !has_juxtaposed(cur + 1)) {
                /* Store the term in the cache. */
                grow_cache(cache_len + 1, &cache, &cache_cap);
                cache[cache_len++] = cur;
                break;                /* advance to find the next term */
            }
            cur++;
        }
    }

    return cache[n - 1];
}
