/*
 * A042858  Numerators of continued fraction convergents to sqrt(960).
 * https://oeis.org/A042858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 62*a(n-2) - a(n-4), a(0)=30, a(1)=31, a(2)=1890,
 *            a(3)=1921
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;

    /* Initialise cache with first four terms once */
    if (cache == NULL)
    {
        cache_capacity = 8;                 /* initial capacity > 4 */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                /* allocation failure, unlikely */

        cache[0] = (seq_t)30;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)1890;
        cache[3] = (seq_t)1921;
        cache_len = 4;
    }

    /* If requested value already cached, return it directly */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure capacity for the requested index */
    size_t target = (size_t)n;
    if (target + 1 > cache_capacity)
    {
        size_t new_capacity = cache_capacity * 2;
        while (new_capacity <= target + 1)
            new_capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                /* realloc failure, unlikely */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Compute values up to target */
    for (size_t i = cache_len; i <= target; ++i)
    {
        /* Recurrence only applies for i >= 4 */
        if (i >= 4)
        {
            /* Since a(i-2) and a(i-4) are already known, compute a(i) */
            cache[i] = (seq_t)62 * cache[i - 2] - cache[i - 4];
        }
        /* For i == 0..3, values are already initialised */
    }
    cache_len = target + 1;

    return cache[target];
}
