/*
 * A164053  Partial sums of A162255.
 * https://oeis.org/A164053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 5 for n > 2 Initial values: a(1) = 3, a(2) = 5
 *            This implements a linear recurrence with memoisation
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* 1‑based cache: cache[1] == a(1) */
static seq_t *cache      = NULL;   /* pointer to the allocated array */
static size_t cache_cap  = 0;      /* allocated size in elements     */
static idx_t  cache_len  = 0;      /* highest index that has been filled */

/* Return a(n) for any n >= 1, calls may come in any order. */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1 – undefined below 1 */
        return (seq_t)0;

    /* Ensure the cache can hold up to index n. */
    size_t needed = (size_t)n + 1;   /* index 0 is unused */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two values on first call. */
    if (cache_len == 0) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache_len = 2;
    }

    /* Compute any missing terms up to n. */
    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* For i == 1 or 2 the value is already set. */
            seq_t val;
            if (i == 1)
                val = (seq_t)3;
            else if (i == 2)
                val = (seq_t)5;
            else
                val = (seq_t)2 * cache[i - 2] + (seq_t)5;
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
