/*
 * A044831  Positive integers having more base-5 runs of even length than odd.
 * https://oeis.org/A044831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_even_runs_more
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers having more base‑5 runs of even length than odd. */
/* Generate by brute force and cache results. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_even_runs_more.h"

static seq_t *cache            = NULL;   /* cached terms            */
static size_t cache_len        = 0;      /* number of cached terms  */
static size_t cache_cap        = 0;      /* allocated capacity      */
static seq_t last_checked      = 0;      /* last integer examined   */

/* Ensure that the cache contains at least n terms.  Expands the cache
   as necessary. */
static void fill_cache(idx_t n)
{
    if (n <= cache_len) return;   /* already enough */

    /* Decide on the new capacity */
    size_t new_cap = cache_cap;
    if (new_cap < n) {
        new_cap = n > 8 ? n * 2 : 8;
    }
    if (new_cap != cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed; abort */
            abort();
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Search for new terms */
    while (cache_len < n) {
        last_checked++;
        if (has_even_runs_more(last_checked)) {
            cache[cache_len++] = last_checked;
        }
    }
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for zero index */
    fill_cache(n);
    return cache[n - 1];
}
