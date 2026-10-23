/*
 * A044837  Positive integers having more base-11 runs of even length than
 *          odd.
 * https://oeis.org/A044837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   more_even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044837: positive integers having more base‑11 runs of even length than odd.
   The algorithm incrementally scans integers, checking the parity of runs in their base‑11 representation
   and caching found terms for future calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/more_even_runs.h"

/* Cached sequence terms */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of stored terms */
static size_t cache_cap = 0;     /* current allocation capacity */
static seq_t last_checked = 0;   /* highest integer already examined */

/* Ensure the cache has room for 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;

    /* allocate new array and copy existing terms */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache_cap) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until we have at least 'target' elements cached. */
static void compute_until(idx_t target)
{
    while (cache_len < (size_t)target) {
        last_checked++;                     /* advance to next integer to test */
        if (more_even_runs(last_checked)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
}

/* External interface required by the platform */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1, so we ignore n==0 */
        return 0;
    compute_until(n);
    return cache[n - 1];
}
