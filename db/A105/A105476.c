/*
 * A105476  Number of compositions of n when each even part can be of two
 *          kinds.
 * https://oeis.org/A105476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), with a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the tiny base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Static cache of already-computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first three terms once. */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* Memory allocation failed. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache_size = 3;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Resize the cache if necessary.  Double its size until it can hold n. */
    idx_t new_size = cache_size * 2;
    if (new_size < n + 1)
        new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;                    /* Memory allocation failed. */
    cache = tmp;

    /* Compute the remaining terms up to index n. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 2];
    }
    cache_size = n + 1;

    return cache[n];
}
