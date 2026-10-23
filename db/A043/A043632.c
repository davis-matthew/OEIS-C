/*
 * A043632  Numbers whose base-9 representation has exactly 3 runs.
 * https://oeis.org/A043632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑9 representation has *
 *            exactly 3 maximal equal‑digit runs
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   count_base9_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_base9_runs.h"

/* static cache of computed terms */
static size_t cache_sz   = 0;          /* number of cached terms */
static size_t cache_cap  = 0;          /* allocated capacity of the cache */
static seq_t *cache      = NULL;      /* pointer to the cached terms */
static idx_t  next_start = 1;         /* next integer to test for the 3‑run property */

/* resize the cache to hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;        /* double until it fits */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();              /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n terms. */
    while (cache_sz < (size_t)n) {
        seq_t cur = (seq_t)next_start;
        if (count_base9_runs(cur) == 3) {
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = cur;
        }
        ++next_start;
    }
    return cache[n - 1];
}
