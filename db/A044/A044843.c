/*
 * A044843  Positive integers having no fewer base-2 runs of even length than
 *          odd.
 * https://oeis.org/A044843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_balanced_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: positive integers having no fewer base‑2 runs of even length
 * than odd.  The function searches integers, tests the run condition,
 * and memoises the results. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_balanced_runs.h"

/* --- static cache ----------------------------------------------------- */
static seq_t *cache = NULL;     /* cached sequence values           */
static size_t cache_cap = 0;    /* current allocation capacity      */
static size_t cache_len = 0;    /* number of values stored          */
static idx_t last_checked = 0;  /* last integer examined            */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap > 0 ? cache_cap * 2 : 16);
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* --- public interface -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0 */
    /* Build cache up to index n */
    while (cache_len < (size_t)n) {
        ++last_checked;                      /* candidate integer */
        if (has_balanced_runs((seq_t)last_checked)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
