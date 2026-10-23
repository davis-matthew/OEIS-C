/*
 * A044835  Positive integers having more base-9 runs of even length than odd.
 * https://oeis.org/A044835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_more_even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = nth positive integer whose base‑9 representation
 * has more runs of even length than runs of odd length.
 * The code generates the sequence by brute‑force search and caches
 * results for repeated or out‑of‑order queries.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/has_more_even_runs.h"

/* Cache of generated terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = 0;

/* Grow cache to hold more items */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory error */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, just return 0 */

    /* generate until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (cache_len >= cache_cap)
            grow_cache();

        /* start candidate from one after the last checked integer */
        seq_t cand = last_checked + 1;
        /* search for next valid integer */
        while (true) {
            if (has_more_even_runs(cand)) {
                cache[cache_len++] = cand;
                ++cand;
                break;
            }
            ++cand;
        }
        last_checked = cand - 1;
    }

    return cache[(size_t)n - 1];
}
