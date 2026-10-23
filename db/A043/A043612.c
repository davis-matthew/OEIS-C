/*
 * A043612  Numbers whose base-6 representation has exactly 4 runs.
 * https://oeis.org/A043612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_four_runs_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers whose base-6 representation has exactly 4 runs
   by scanning consecutive integers and caching the results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_four_runs_idx.h"

/* Static state for memoisation */
static size_t cache_cap   = 0;
static seq_t *cache       = NULL;
static size_t cache_len   = 0;
static idx_t last_checked = 1;   /* next integer to test */

/* Ensure the cache can hold at least `needed` entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for n=0 */

    while (cache_len < n)
    {
        grow_cache(cache_len + 1);
        if (has_four_runs_idx(last_checked))
            cache[cache_len++] = (seq_t)last_checked;
        ++last_checked;
    }
    return cache[n - 1];
}
