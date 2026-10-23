/*
 * A131323  Odd numbers whose binary expansion ends in an even number of 1's.
 * https://oeis.org/A131323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   trailing_ones_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A131323: odd numbers whose binary expansion ends
   with an even number of consecutive 1's.  The implementation
   iterates over odd numbers, checks the length of trailing 1's and
   keeps the values in a cache so that any call order is supported. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/trailing_ones_even.h"

/* static cache to hold computed terms */
static seq_t *cache = NULL;      /* array of terms */
static idx_t cache_cap = 0;      /* allocated capacity (in terms) */
static idx_t cache_len = 0;      /* number of terms actually stored */

/* last odd number that was tested (initially 1, before the first odd
   candidate 3) */
static seq_t last_tested = 1;

/* Ensure the cache can hold at least 'needed' terms.  The capacity
   grows geometrically to avoid frequent reallocations. */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Returns the nth term of A131323 (n >= 1).  Subsequent calls may be
   in any order; computed terms are cached. */
seq_t a(idx_t n)
{
    /* Validate that n is at least 1; behaviour for n<1 is undefined. */
    /* Fill the cache up to index n. */
    ensure_cache_cap(n);
    while (cache_len < n) {
        last_tested += 2;     /* only odd candidates */
        if (trailing_ones_even(last_tested)) {
            cache[cache_len] = last_tested;
            cache_len++;
        }
    }
    return cache[n - 1];
}
