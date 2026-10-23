/*
 * A044379  Numbers n such that string 4,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains47
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose decimal representation contains the
   substring "47" but (n − 1) does not.  The implementation performs a
   linear search and memoizes previously found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/contains47.h"

/* Memoization state.  */
static seq_t  *cache     = NULL;   /* array of found terms */
static size_t cache_size  = 0;     /* number of terms currently stored */
static size_t cache_cap   = 0;     /* allocated capacity of 'cache' */
static idx_t  next_candidate = 1;  /* next integer to examine */

/* Ensure that cache contains at least n terms (a(1) … a(n)). */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;                     /* already have enough terms */

    /* Grow the cache array if necessary. */
    while (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(*newp));
        cache     = newp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_size < (size_t)n) {
        if (contains47(next_candidate) && !contains47(next_candidate - 1)) {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Main entry point required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below the offset is irrelevant */

    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
