/*
 * A110502  Numbers n such that n in binary representation has a block of
 *          exactly a nontrivial square number of zeros.
 * https://oeis.org/A110502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_square_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// OEIS A110502: numbers whose binary representation contains a block
// of exactly k^2 zeros for some integer k > 1. The sequence is generated
// by brute‑force search: we test each integer in increasing order and
// keep those that satisfy the property.  Cached values are stored so
// repeated calls are fast.

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_square_block.h"

/* Cached results of the sequence.  cache_size holds the number of
   already computed terms; cache_cap is the allocated capacity. */
static seq_t *cache         = NULL;
static size_t cache_size    = 0;
static size_t cache_cap     = 0;

/* Next integer to test.  Starts at 1. */
static seq_t next_candidate = 1;

/* Ensure the cache has at least needed_capacity elements. */
static void ensure_cache_capacity(size_t needed_capacity)
{
    if (needed_capacity <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed_capacity)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();   /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of A110502 (n ≥ 1).  The function is
   memoised, so repeated calls are efficient. */
seq_t a(idx_t n)
{
    /* n is 1‑based.  Ensure we have computed at least n terms. */
    while (cache_size < (size_t)n) {
        if (has_square_block(next_candidate)) {
            /* Store the term. */
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
