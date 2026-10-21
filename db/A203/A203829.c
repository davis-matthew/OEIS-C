/*
 * A203829  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=225, a(2)=1971, a(n)=9*a(n-1)-2*a(n-2) for n≥3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for n==0 to keep the function well‑defined, though
       the sequence is only defined for n≥1. */
    if (n == 0) return (seq_t)0;

    /* Handle the two base cases without any extra allocation. */
    if (n == 1) return (seq_t)225;
    if (n == 2) return (seq_t)1971;

    /* Static cache for memoisation; cache[i] holds a(i+1). */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;    /* number of terms stored (>=2) */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Initialise the cache on first use or if the current allocation
       is too small for the requested index. */
    if (cache == NULL || cache_cap < (size_t)n)
    {
        size_t new_cap = (cache_cap == 0) ? (size_t)n + 1 : cache_cap;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

        /* Allocate or grow the cache. */
        if (cache == NULL)
        {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0;   /* allocation failure */
            cache[0] = (seq_t)225;
            cache[1] = (seq_t)1971;
            cache_len = 2;
        }
        else
        {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;     /* reallocation failure */
            cache = tmp;
        }
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to the requested index. */
    while (cache_len < (size_t)n)
    {
        seq_t prev  = cache[cache_len - 1];   /* a(k)   where k = cache_len   */
        seq_t prev2 = cache[cache_len - 2];   /* a(k-1) where k = cache_len   */
        cache[cache_len] = (seq_t)9 * prev - (seq_t)2 * prev2;
        ++cache_len;
    }

    return cache[n - 1];
}
