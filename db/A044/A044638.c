/*
 * A044638  Numbers n such that string 0,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_substring_06
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code for OEIS A044638: numbers n such that the base‑9 representation of
   n contains the substring "06" but that of n+1 does not.  The sequence is
   indexed with offset 1. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint8_t */
#include <stdbool.h>
#include <stdlib.h>   /* malloc, realloc, exit */
#include "../utils/has_substring_06.h"

/* Dynamic cache for generated sequence values */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cached_cnt = 0;    /* number of terms stored in cache */
static idx_t  cache_cap = 0;     /* allocated capacity of cache */
static idx_t  next_candidate = 0;/* next integer to test */

/* Ensure the cache can hold at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache)
        exit(1);           /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1; guard for safety */
        return (seq_t)0;

    /* Generate terms up to the required index, if necessary. */
    if (n > cached_cnt) {
        grow_cache(n);
        while (cached_cnt < n) {
            if (has_substring_06(next_candidate) &&
                !has_substring_06(next_candidate + 1)) {
                cache[cached_cnt++] = (seq_t)next_candidate;
            }
            next_candidate++;
        }
    }

    /* Return the cached value (offset 1 → index n-1). */
    return cache[n - 1];
}
