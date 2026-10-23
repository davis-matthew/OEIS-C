/*
 * A044381  Numbers n such that string 4,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains49
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers n such that the decimal representation of n contains
 * the substring "49" but the decimal representation of n-1 does not.
 * Computation is performed by incremental search with memoisation.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/contains49.h"

/* -------------------------------------------------------------------- */
/* Helper: test whether a number contains the substring "49" in its    */
/* decimal representation.                                              */

/* -------------------------------------------------------------------- */
/* Static state: cache of previously found terms and auxiliary counters */
/* -------------------------------------------------------------------- */
static seq_t *cache     = NULL; /* array of found terms                 */
static size_t cache_cap = 0;    /* allocated capacity of the array     */
static size_t cache_len = 0;    /* number of terms stored in the cache */
static seq_t last_checked = 1;  /* the next integer to test           */

/* -------------------------------------------------------------------- */
/* Grow the cache to hold at least 'needed' elements.                   */
/* -------------------------------------------------------------------- */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)
            abort(); /* unrecoverable allocation failure */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* -------------------------------------------------------------------- */
/* The required function: a(idx_t n)                                   */
/* Returns the nth number in the sequence (1‑based indexing).          */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Fast path: return cached value if already known. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate terms until we reach the desired index. */
    for (/* no init */; cache_len < (size_t)n; ++last_checked)
    {
        /* Check if current number satisfies the condition and that its
         * predecessor does not. */
        if (contains49(last_checked) &&
            !contains49(last_checked - 1))
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }

    return cache[(size_t)n - 1];
}
