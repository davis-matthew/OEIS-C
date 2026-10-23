/*
 * A122973  Number of vertices on the surface of an icosahedron.
 * https://oeis.org/A122973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=12, a(2)=42, a(n)=5*a(n-1)-4*a(n-2) for n>=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* pointer to cached values   */
    static idx_t cache_len = 0;      /* number of cached entries   */
    static idx_t cache_cap = 0;      /* allocated capacity         */

    /* initialize cache on first call */
    if (cache_cap == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed: return 0 (unlikely) */
        cache[0] = (seq_t)1;        /* a(0) = 1   */
        cache[1] = (seq_t)12;       /* a(1) = 12  */
        cache[2] = (seq_t)42;       /* a(2) = 42  */
        cache_len = 3;              /* we have 0,1,2 cached  */
    }

    /* extend cache if necessary */
    if ((idx_t)n >= cache_len) {
        idx_t i, new_cap = cache_cap;
        /* grow capacity until it can hold n+1 entries */
        while (new_cap <= (idx_t)n)
            new_cap *= 2;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;    /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms using the recurrence */
        for (i = cache_len; i <= (idx_t)n; ++i) {
            /* a(i) = 5*a(i-1) - 4*a(i-2)   (valid for i>=3) */
            cache[i] = (cache[i - 1] * (seq_t)5) - (cache[i - 2] * (seq_t)4);
        }
        cache_len = (idx_t)n + 1; /* we now have entries up to n */
    }

    /* return the requested value */
    return cache[(idx_t)n];
}
