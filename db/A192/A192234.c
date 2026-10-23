/*
 * A192234  a(n) = 2*(a(n-1) + a(n-2) + a(n-3)) - a(n-4) for n >= 4, with
 *          initial terms 0,1,0,1.
 * https://oeis.org/A192234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=0, a(3)=1,  a(n)=2*(a(n-1)+a(n-2)+a(n-3))-a(n-4) for n≥4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache    = NULL;   /* array of seq_t values */
    static size_t cap      = 0;      /* allocated capacity (# elements) */
    static size_t computed = 0;      /* number of terms already computed */

    /* Ensure cache can hold index n */
    if ((size_t)n >= cap)
    {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) exit(1);            /* exit on allocation failure */

        if (cache)
        {
            /* copy previously computed values */
            for (size_t i = 0; i < computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)0;
        else if (i == 3)
            cache[i] = (seq_t)1;
        else
        {
            seq_t t = cache[i-1] + cache[i-2] + cache[i-3];
            cache[i] = (seq_t)2 * t - cache[i-4];
        }
    }
    computed = (size_t)n + 1;
    return cache[n];
}
