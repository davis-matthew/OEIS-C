/*
 * A045145  Numbers whose base-4 representation contains exactly two 2's and
 *          two 3's.
 * https://oeis.org/A045145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base-4 representation contains
 *            exactly two 2's and two 3's. This program searches integers in
 *            increasing order, caching results for reuse
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_two_twos_and_twos_threes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_two_twos_and_twos_threes.h"

/* Dynamic cache for computed terms. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;

/* Next candidate to test.  Starts at 0 and moves forward as
   we find terms. */
static idx_t next_candidate = 0;

/* Ensure the cache has room for at least *need* entries. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined offset, return 0 as a placeholder */

    /* Compute terms until we have at least n of them. */
    while (cache_len < n) {
        while (!has_two_twos_and_twos_threes(next_candidate))
            ++next_candidate;

        /* Store the found term. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
