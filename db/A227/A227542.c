/*
 * A227542  a(n) is the number of all terms preceding a(n-1) that have the
 *          same even-odd parity as a(n-1).
 * https://oeis.org/A227542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227542: a(n) = number of preceding terms with the same parity
   as a(n-1); a(0)=0.  For n>0 a(n) = (a(n-1) even) ? (even_count-1)
   : (odd_count-1) where even_count/odd_count are counts of even/odd terms
   among the already computed terms.  The function keeps a static cache of
   computed values so that calls in arbitrary order are handled quickly. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;           /* values a(k) for k < cache_len */
static idx_t cache_len = 0;           /* number of stored elements */
static idx_t cache_cap = 0;           /* allocated capacity */
static seq_t even_count = 0;          /* number of even terms in cache */
static seq_t odd_count  = 0;          /* number of odd  terms in cache */

seq_t a(idx_t n)
{
    /* initialise the cache with a(0)=0 */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache_len = 1;
        even_count = 1;      /* a(0) is even */
        odd_count  = 0;
    }

    /* extend the cache up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i-1];
            bool prev_even = ((prev & 1) == 0);

            /* count of preceding terms with the same parity as prev */
            seq_t next = prev_even ? (even_count - 1) : (odd_count - 1);

            /* grow the array if necessary */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }

            cache[cache_len] = next;
            /* update parity counters for the newly added term */
            if ((next & 1) == 0)
                ++even_count;
            else
                ++odd_count;
            ++cache_len;
        }
    }

    return cache[n];
}
