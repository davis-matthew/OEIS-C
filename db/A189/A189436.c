/*
 * A189436  Number of 4 X n array permutations with each element not moving,
 *          or moving one space N, SW or SE.
 * https://oeis.org/A189436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 6*a(n-3) + 4*a(n-4) - a(n-5)
 *            Initial values: a(1)=1, a(2)=9, a(3)=31, a(4)=140, a(5)=571
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 5;

    if (n < 1)
        return 0;   /* undefined for n < 1, but not expected */

    /* Initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 6;               /* indices 0 .. 5 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return 0;                /* allocation failure */
        cache[0] = 0;                /* unused */
        cache[1] = 1;                /* a(1) */
        cache[2] = 9;                /* a(2) */
        cache[3] = 31;               /* a(3) */
        cache[4] = 140;              /* a(4) */
        cache[5] = 571;              /* a(5) */
    }

    /* Grow cache if necessary */
    if ((idx_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((idx_t)n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            cache[i] = 4 * cache[i-1]
                     + 2 * cache[i-2]
                     - 6 * cache[i-3]
                     + 4 * cache[i-4]
                     - cache[i-5];
        }
        last_computed = n;
    }

    return cache[n];
}
