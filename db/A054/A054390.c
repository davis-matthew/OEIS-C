/*
 * A054390  Number of ways of writing n as a sum of powers of 3, each power
 *          being used at most three times.
 * https://oeis.org/A054390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   * a(0)=1, a(1)=1, a(2)=1 * for n>0: * let q = n/3 and r = n%3 *
 *            if r==0 a(n) = a(q) + a(q-1) * else a(n) = a(q) * This
 *            recurrence follows directly from the OEIS definition of A054390
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

/* C and C++ compatible implementation */
seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t cache_size = 0;  /* number of valid entries in cache */

    /* initialise cache with the base terms a(0), a(1), a(2) */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure – give a default value */
            return 0;
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache_size = 3;
    }

    /* ensure the cache contains all values up to n */
    if (n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)            /* allocation failure – return default */
            return 0;
        cache = tmp;

        /* compute values from old cache_size up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i)
        {
            if (i < 3)   /* base terms already set */
                continue;

            idx_t q = (idx_t)(i / 3);
            idx_t r = (idx_t)(i % 3);

            if (r == 0)
            {
                /* a(3q) = a(q) + a(q-1), q>=1 here because i>=3 */
                cache[i] = cache[q] + cache[q - 1];
            }
            else
            {
                /* a(3q+1) or a(3q+2) = a(q) */
                cache[i] = cache[q];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
