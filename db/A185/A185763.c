/*
 * A185763  Number of (n+1) X 4 binary arrays with no 2 X 2 subblock trace
 *          equal to any horizontal or vertical neighbor 2 X 2 subblock trace.
 * https://oeis.org/A185763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 10*a(n-2) + 3*a(n-3) - 5*a(n-4) + a(n-5),
 * for n > 6
 * Initial values: a(1)=104, a(2)=368, a(3)=1408, a(4)=5272, a(5)=19992, a(6)=75192
 */
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* cache[0] unused; indices 1..n directly address terms */
    static seq_t *cache = NULL;          /* allocated cache */
    static idx_t cached_len = 0;         /* number of cache elements allocated minus 1 */

    if (n < 1) return (seq_t)0;          /* not defined for n<1, return 0 */

    /* ensure cache has at least n+1 elements (index 0 unused) */
    if (cache == NULL)
    {
        /* allocate space for indices 0..6 to store the first 6 values initially */
        idx_t init_len = 7;  /* 0..6 */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cached_len = init_len - 1;
        /* initialise first 6 values */
        cache[1] = (seq_t)104;
        cache[2] = (seq_t)368;
        cache[3] = (seq_t)1408;
        cache[4] = (seq_t)5272;
        cache[5] = (seq_t)19992;
        cache[6] = (seq_t)75192;
    }
    else if (n > cached_len)
    {
        /* enlarge cache to hold up to n */
        idx_t new_len = n + 1; /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* realloc failed */
        cache = tmp;
        /* initialise any new indices that don't yet exist up to cache_len */
        for (idx_t i = cached_len + 1; i <= n; ++i)
        {
            if (i <= 6)
            {
                /* already initialised when cache was first allocated */
                continue;
            }
            /* compute recurrence for i >= 7 */
            cache[i] = cache[i-1] +
                       ((seq_t)10 * cache[i-2]) +
                       ((seq_t)3  * cache[i-3]) -
                       ((seq_t)5  * cache[i-4]) +
                       cache[i-5];
        }
        cached_len = n;
    }

    return cache[n];
}
