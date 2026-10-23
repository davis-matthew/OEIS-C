/*
 * A017898  Expansion of (1-x)/(1-x-x^4).
 * https://oeis.org/A017898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 74, u64 n <= 143,
 *            u128 n <= 281, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A017898
 *  Formula: a(n) = a(n-1) + a(n-4),
 *  initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0
 *
 *  Computes a(n) for any n ≥ 0.  The result is memoised in a
 *  static dynamic array so that calls in arbitrary order remain
 *  efficient.  All arithmetic stays in the type seq_t; no overflow
 *  occurs for values that fit in seq_t (arbitrary‑precision build
 *  handles larger values automatically).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* number of elements currently allocated */

    /* Ensure the first four terms are always present. */
    if (cap < 4) {
        cache = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
        cap   = 4;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
    }

    /* For n < 4 the value is already initialised. */
    if (n < 4)
        return cache[(size_t)n];

    /* If we have not yet computed up to n, extend the cache and fill. */
    if ((size_t)n >= cap) {
        /* Grow the array to hold indices 0..n. */
        cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        /* Compute the new terms, starting from the first index
         * that is currently undefined.  The loop variable uses
         * size_t to avoid signed/unsigned mismatches. */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-4];
        }
        cap = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
