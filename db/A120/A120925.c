/*
 * A120925  Number of ternary words on {0,1,2} having no isolated 0's.
 * https://oeis.org/A120925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3); Initial values: a(0)=1,
 *            a(1)=2, a(2)=5;
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t  computed_upto = (idx_t)-1;   /* last index whose value is known */

    /* enlarge storage if needed */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the base values (once or after an expansion) */
    if (computed_upto == (idx_t)-1)            /* first call */
    {
        cache[0] = 1;
        if (cache_cap > 1) cache[1] = 2;
        if (cache_cap > 2) cache[2] = 5;
        computed_upto = (cache_cap > 2) ? 2
                          : (cache_cap > 1) ? 1
                                            : 0;
    }
    else if (computed_upto < 2)                /* we have grown capacity */
    {
        if (cache_cap > 1) cache[1] = 2;
        if (cache_cap > 2) cache[2] = 5;
        computed_upto = (cache_cap > 2) ? 2
                          : (cache_cap > 1) ? 1
                                            : 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i)
    {
        /* recurrence: a(i) = 3*a(i-1) - 2*a(i-2) + 2*a(i-3) */
        cache[i] = 3 * cache[i-1];
        cache[i] = cache[i] - 2 * cache[i-2];
        cache[i] = cache[i] + 2 * cache[i-3];
    }
    computed_upto = n;

    return cache[n];
}
