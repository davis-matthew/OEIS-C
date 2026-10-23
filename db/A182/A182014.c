/*
 * A182014  Number of independent sets of nodes in graph C_7 x P_n (n>=0).
 * https://oeis.org/A182014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A182014: Number of independent sets of nodes in graph C_7 × P_n
   Recurrence: a(n) = 17*a(n-1) + 8*a(n-2) - 44*a(n-3) + 5*a(n-4) + a(n-5)
   Initial values: a(0)=1, a(1)=29, a(2)=477, a(3)=8303, a(4)=143697 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;

    /* initialise cache with base values if empty */
    if (!len)
    {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed, return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)477;
        cache[3] = (seq_t)8303;
        cache[4] = (seq_t)143697;
        len = 5;
    }

    /* if requested n is already cached, just return it */
    if (n < len)
    {
        return cache[(size_t)n];
    }

    /* grow capacity as needed */
    if ((size_t)n >= cap)
    {
        size_t new_cap = (cap == 0) ? 5 : cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i)
    {
        seq_t val = 17 * cache[(size_t)(i-1)]
                  + 8 * cache[(size_t)(i-2)]
                  - 44 * cache[(size_t)(i-3)]
                  + 5 * cache[(size_t)(i-4)]
                  +     cache[(size_t)(i-5)];
        cache[(size_t)i] = val;
    }
    len = n + 1;

    return cache[(size_t)n];
}
