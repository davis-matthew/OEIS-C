/*
 * A039281  Numbers whose base-4 representation has the same nonzero number of
 *          1's and 3's.
 * https://oeis.org/A039281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose base‑4 representation
 *            contains equal positive counts of the digits 1 and 3 (the other
 *            digits are ignored)
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_equal_ones_and_threes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/has_equal_ones_and_threes.h"

/* Global cache for computed terms and the next number to examine. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate;

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2U : 8U;
    while (new_cap < needed) new_cap *= 2U;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);               /* abort on allocation failure */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but keeps compiler happy */

    /* Initialise global state on first call. */
    if (!cache) {
        cache_cap   = 8U;
        cache_len   = 0U;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        next_candidate = (seq_t)1;
    }

    /* Generate values until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t val = next_candidate;
        if (has_equal_ones_and_threes(val)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        next_candidate += (seq_t)1;
    }

    return cache[(size_t)n - 1U];
}
