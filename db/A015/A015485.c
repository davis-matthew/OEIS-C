/*
 * A015485  q-Fibonacci numbers for q=12, scaling a(n-1).
 * https://oeis.org/A015485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(n-1)*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

/* seq_t and idx_t are supplied by the build */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of stored elements (size of cache)   */
    static seq_t pow12 = (seq_t)0;/* 12^(k) where k = index of last computed a(n) */

    /* Handle base cases directly to avoid unnecessary allocation */
    if (n == 0) { return (seq_t)0; }
    if (n == 1) { return (seq_t)1; }

    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache   = (seq_t *)realloc(NULL, 2 * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
        pow12     = (seq_t)12;          /* 12^(1) for computing a(2)                      */
    }

    /* If the requested term already exists, return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Ensure the cache is large enough to store up to a(n) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed; behaviour not defined, return 0 as safety */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute values from cache_len up to n */
    size_t i_start  = cache_len;                     /* first new index to compute */
    seq_t  prev2    = cache[i_start - 2];            /* a(i_start-2)                */
    seq_t  prev1    = cache[i_start - 1];            /* a(i_start-1)                */
    seq_t  p        = pow12;                         /* 12^(i_start-1)              */

    for (size_t i = i_start; i <= (size_t)n; ++i) {
        seq_t ai = p * prev1 + prev2;                /* recurrence               */
        cache[i] = ai;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = ai;
        p *= (seq_t)12;                              /* next 12^(i)              */
    }

    /* Update cache state for future calls */
    cache_len = new_len;
    pow12     = p;                                  /* now holds 12^n             */

    return cache[n];
}
