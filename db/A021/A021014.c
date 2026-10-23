/*
 * A021014  a(n)=a(n-1)+a(n-2)-a(n-4)+a(n-5).
 * https://oeis.org/A021014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) - a(n-4) + a(n-5)
   Initial values: a(0)=7, a(1)=11, a(2)=17, a(3)=26, a(4)=40
   Offset: 0 (a(0) is the first term) */
#include <stddef.h>   /* NULL, size_t   */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache     = NULL;   /* array holding computed terms   */
    static size_t cache_len = 0;      /* number of values stored        */
    static size_t cache_cap = 0;      /* allocated capacity (in elements) */

    /* Convert index to a size_t suitable for array access */
    size_t idx = (size_t)n;

    /* --- initialise cache on first call -------------------------------- */
    if (cache == NULL)
    {
        /* Start with capacity for the five known initial values. */
        cache_cap = 5;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)   /* allocation failed: never expected, but be safe */
            return (seq_t)0;

        /* Store the initial values */
        cache[0] = (seq_t)7;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)40;
        cache_len = 5;
    }

    /* --- enlarge cache if the requested index is beyond current capacity */
    if (idx + 1 > cache_cap)
    {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < idx + 1)  /* grow until it fits idx */
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)          /* realloc failed; keep old cache for safety */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* --- compute any missing terms up to idx -------------------------------- */
    if (idx + 1 > cache_len)
    {
        /* Start computing from the first missing index */
        for (size_t i = cache_len; i <= idx; ++i)
        {
            /* The recurrence is valid for i >= 5.  For i < 5 the values are
               already stored above. */
            cache[i] = cache[i-1] + cache[i-2] - cache[i-4] + cache[i-5];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
