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
 * Validated: standard, pass, 69 terms through n = 9740 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Helper: return true iff x has a maximal run of zeros whose length
   is a perfect square greater than 1. */
static bool has_square_block(seq_t x)
{
    if (x == 0)
        return false;

    seq_t tmp = x;
    while (tmp) {
        /* Skip trailing ones to find the start of a zero run. */
        while ((tmp & 1) && tmp) {
            tmp >>= 1;
            if (!tmp)
                return false;      /* number ended with ones only */
        }
        if (!tmp)                 /* no zeros left */
            break;

        /* Count consecutive zeros. */
        unsigned long long run = 0;
        while (!(tmp & 1) && tmp) {
            ++run;
            tmp >>= 1;
        }

        /* Check whether the run length is a non‑trivial square. */
        if (run >= 4) {           /* smallest square > 1 is 4 */
            unsigned long long s = 0;
            while ((s + 1) * (s + 1) <= run)
                ++s;
            if (s * s == run)
                return true;
        }
    }
    return false;
}

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
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
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
