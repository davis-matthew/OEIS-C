/*
 * A043110  Numbers k such that 1 and 2 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_adj_12_or_21
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043110 – Numbers k such that 1 and 2 occur juxtaposed in the
   base‑5 representation of k but not of k‑1.
   Computation: linear search with memoisation; each integer is
   tested for the adjacency pattern. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj_12_or_21.h"

/* Dynamic cache of found sequence elements */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_start  = (seq_t)1;   /* next integer to test */

/* Grow the cache array to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 16;
    while (cap < needed) cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!new_arr) exit(1);              /* abort on allocation failure */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        free(cache);
    }
    cache     = new_arr;
    cache_cap = cap;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset */

    size_t target = (size_t)n;
    if (target > cache_len) {
        while (cache_len < target) {
            seq_t cand = next_start++;                     /* test successive integers */
            if (has_adj_12_or_21(cand) &&                /* cand has adjacency */
                !has_adj_12_or_21(cand - (seq_t)1)) {      /* but cand-1 does not */
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
            }
        }
    }
    return cache[n - 1];
}
