/*
 * A082574  a(1)=1, a(n) = ceiling(r(3)*a(n-1)) where r(3) = (1/2)*(3 +
 *          sqrt(13)) is the positive root of X^2 = 3*X + 1.
 * https://oeis.org/A082574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=14, a(n)=4*a(n-1)-2*a(n-2)-a(n-3) (n>3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises computed terms so that calls in arbitrary order run in O(n) time. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of cache array */
    static idx_t cache_len = 0;   /* highest index stored (a[cache_len]) */

    /* Ensure cache is large enough to hold indices up to n */
    if (cache_len < n)
    {
        /* Initialise cache if first call */
        if (cache_len == 0)
        {
            cache_cap = 4;                      /* minimal capacity from a(1)..a(3) */
            cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t)); /* +1 because we use 1‑based indexing */
            if (!cache) return (seq_t)0;        /* allocation failure, return 0 */

            /* initial values */
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)14;
            cache_len = 3;
        }

        /* Grow cache if needed */
        while (cache_len < n)
        {
            if (cache_len + 2 > cache_cap)
            {
                /* double capacity */
                size_t new_cap = cache_cap * 2 + 1;
                seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }

            /* Compute next term using recurrence */
            seq_t term = (seq_t)4 * cache[cache_len]
                        - (seq_t)2 * cache[cache_len - 1]
                        - cache[cache_len - 2];
            cache[cache_len + 1] = term;
            ++cache_len;
        }
    }

    return cache[n];
}
