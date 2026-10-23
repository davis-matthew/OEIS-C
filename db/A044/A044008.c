/*
 * A044008  Numbers k such that 1 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044008: numbers k such that the decimal representation of k
 * contains the adjacent digits '15' or '51', but the decimal representation
 * of k+1 does not.  The function builds the sequence lazily and caches
 * results so that calls with arbitrary n are answered in O(1) after the
 * first traversal. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent15.h"

static seq_t *cache = NULL;        /* stored sequence terms */
static idx_t  cache_len = 0;       /* number of terms in cache      */
static idx_t  cache_cap = 0;       /* allocated slots in cache      */
static seq_t  next_num  = (seq_t)1;/* next integer to examine       */

/* Grow the cache by doubling its capacity (or starting at 16). */
static void grow_cache(void)
{
    idx_t new_cap = cache_cap ? cache_cap * 2 : (idx_t)16;
    seq_t *newp   = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) exit(1); /* allocation failure; abort */
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i) newp[i] = cache[i];
        free(cache);
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialize cache on first call. */
    if (!cache) {
        cache_len = 0;
        cache_cap = 0;
        next_num  = (seq_t)1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        if (has_adjacent15(next_num) && !has_adjacent15(next_num + (seq_t)1)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = next_num;
        }
        next_num++;
    }

    return cache[n - 1];
}
