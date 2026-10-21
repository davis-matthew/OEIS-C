/*
 * A062188  a(n+1) = a(n) + a(floor(n/2)), with a(0)=0, a(1)=1.
 * https://oeis.org/A062188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor((n-1)/2)), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 27, u16 n <= 109, u32 n <= 707, u64 n <= 9217,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A062188. */
seq_t a(idx_t n)
{
    /* Static state shared by all calls. */
    static seq_t *cache = NULL;          /* array of computed terms          */
    static size_t alloc_len   = 0;        /* number of slots currently allocated */
    static size_t max_computed = 0;       /* highest index that has been computed */

    /* Initialise the cache for indices 0 and 1. */
    if (alloc_len == 0) {
        alloc_len   = 2;
        cache       = (seq_t *)malloc(alloc_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* graceful failure on OOM */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* If n is less than or equal to a previously computed value,
       simply return it. */
    if ((size_t)n <= max_computed) {
        return cache[(size_t)n];
    }

    /* Ensure that the allocation is large enough for indices up to n. */
    if ((size_t)n + 1 > alloc_len) {
        size_t new_alloc = alloc_len ? alloc_len << 1 : 2;
        if (new_alloc < (size_t)n + 1) new_alloc = (size_t)n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* graceful failure on OOM */
        cache = tmp;
        alloc_len = new_alloc;
    }

    /* Compute any missing terms in order. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[(i - 1) / 2];
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
