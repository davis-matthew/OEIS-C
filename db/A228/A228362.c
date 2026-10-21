/*
 * A228362  The number of all possible covers of L-length line segment by
 *          3-length line segments with allowed gaps < 3.
 * https://oeis.org/A228362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-5) for n ≥ 8; a(0) = a(1) = a(2) =
 *            0, a(3) = 1, a(4) = 2, a(5) = 3, a(6) = 3, a(7) = 4
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* array of seq_t */
    static idx_t   capacity = 0;  /* allocated size */

    /* if the value is already in the cache, return it */
    if (n < capacity)
        return cache[n];

    /* enlarge the cache to hold up to index n (size = n+1) */
    idx_t newsize = n + 1;
    seq_t *newptr = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!newptr)
    {
        /* allocation failure: abort gracefully */
        return 0;
    }
    cache   = newptr;
    capacity = newsize;

    /* compute new entries starting from the first unset index */
    for (idx_t i = 0; i < newsize; ++i)
    {
        if (i <= 2)
            cache[i] = 0;
        else if (i == 3)
            cache[i] = 1;
        else if (i == 4)
            cache[i] = 2;
        else if (i == 5)
            cache[i] = 3;
        else if (i == 6)
            cache[i] = 3;
        else if (i == 7)
            cache[i] = 4;
        else
            cache[i] = cache[i - 3] + cache[i - 4] + cache[i - 5];
    }

    return cache[n];
}
