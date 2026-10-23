/*
 * A044844  Positive integers having no fewer base-3 runs of even length than
 *          odd.
 * https://oeis.org/A044844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_valid_base3_runs, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044844: positive integers whose number of runs of even length
   in base‑3 is not fewer than the number of runs of odd length.  The
   following function enumerates integers in increasing order and selects
   those that satisfy the property.  It stores the first terms in a static
   cache, so that arbitrary (non‑monotonic) queries are answered in
   amortised O(1) time. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has_valid_base3_runs.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;

/* Ensure the cache has capacity for at least 'needed' terms. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = (cache_cap == 0) ? 16 : cache_cap;
    while (newcap < needed) newcap <<= 1;
    cache = seq_realloc(cache, cache_cap, newcap);
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; the sequence has offset 1. */
    if (n == (idx_t)0) return (seq_t)0;

    while (cache_len < n) {
        if (has_valid_base3_runs(last_checked)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
        last_checked++;
    }
    return cache[n - 1];
}
