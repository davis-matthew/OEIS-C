/*
 * A123219  Expansion of -x*(x^4 + 52*x^3 - 122*x^2 - 28*x + 1) / ((x-1)*(x^2
 *          - 34*x + 1)*(x^2 + 6*x + 1)).
 * https://oeis.org/A123219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 29*a(n-1)+174*a(n-2)-174*a(n-3)-29*a(n-4)+a(n-5) for n >=
 *            6; a(1)=1, a(2)=1, a(3)=81, a(4)=2401, a(5)=83521
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1 is the first term */
    if (n < 1)
        return (seq_t)0;

    /* static cache indexed from 1 (0 unused) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* highest computed index */

    /* initialise first 5 terms if needed */
    if (cache_size < 5)
    {
        size_t new_sz = 6;          /* indices 0..5 */
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)81;
        cache[4] = (seq_t)2401;
        cache[5] = (seq_t)83521;
        cache_size = 5;
    }

    /* extend cache up to n if necessary */
    if ((size_t)n > cache_size)
    {
        size_t new_sz = (size_t)n + 1;  /* include index n */
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));

        /* compute missing terms using the recurrence */
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i)
        {
            seq_t t1 = ((seq_t)29) * cache[i-1];
            seq_t t2 = ((seq_t)174) * cache[i-2];
            seq_t t3 = ((seq_t)174) * cache[i-3];
            seq_t t4 = ((seq_t)29) * cache[i-4];
            cache[i] = t1 + t2 - t3 - t4 + cache[i-5];
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n];
}
