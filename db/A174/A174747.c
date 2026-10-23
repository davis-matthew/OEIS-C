/*
 * A174747  x-values in the solution to x^2-37*y^2=1.
 * https://oeis.org/A174747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 146*a(n-1) - a(n-2), with a(1)=1, a(2)=73. The sequence
 *            satisfies the Pell equation x^2-37*y^2=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;   /* maximum index currently stored */

    /* initialise cache with first two terms if needed */
    if (cache_len == 0)
    {
        /* allocate space for indices 0..2 (index 0 unused) */
        cache = (seq_t *)malloc((size_t)3 * sizeof(seq_t));
        if (!cache)   /* out of memory */
            return (seq_t)0;   /* undefined behaviour for caller, but unlikely */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)73;
        cache_len = 2;
    }

    /* if requested term already cached, return it */
    if (n <= cache_len)
        return cache[n];

    /* ensure the array is large enough */
    size_t new_capacity = (size_t)n + 2;   /* indices 0..n (0 unused) */
    if ((size_t)(cache_len + 1) < new_capacity)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp)   /* out of memory */
            return (seq_t)0;   /* undefined behaviour for caller */
        cache = tmp;
    }

    /* compute missing terms iteratively */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* recurrence relation */
        cache[i] = 146 * cache[i - 1] - cache[i - 2];
    }

    cache_len = n;
    return cache[n];
}
