/*
 * A090408  a(n) = Sum_{k=0..n} binomial(4*n+3,4*k).
 * https://oeis.org/A090408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=36, a(n)=12*a(n-1)+64*a(n-2) for n≥2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size  = 0;  /* number of cached terms */
    static size_t cap   = 0;  /* capacity of cache array */

    /* Handle the first two values directly (no cache needed yet). */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)36;

    /* Initialise cache on first call. */
    if (cache == NULL)
    {
        /* Reserve at least enough space for n + 1 elements. */
        cap = (n > 1) ? (n + 1) : 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure, unlikely */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)36;
        size   = 2;
    }

    /* Grow the cache if we need more space. */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n + 1 > size)
    {
        for (size_t i = size; i <= (size_t)n; ++i)
        {
            cache[i] = (seq_t)( (seq_t)12 * cache[i-1] + (seq_t)64 * cache[i-2] );
        }
        size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
