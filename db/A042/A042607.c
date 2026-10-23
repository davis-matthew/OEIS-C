/*
 * A042607  Denominators of continued fraction convergents to sqrt(832).
 * https://oeis.org/A042607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(832).
   Recurrence: Q(-2)=1, Q(-1)=0,
               a_0 = 28,
               a_n for n>=1 follows the period 1,5,2,2,1,13,1,2,2,5,1,56,
               Q_n = a_n * Q_{n-1} + Q_{n-2}   (n>=0).
   a(n) = Q_n, with a(0)=1, a(1)=1. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed denominators */
    static seq_t *cache      = NULL; /* array storing Q_0, Q_1, … */
    static size_t cache_cap  = 0;    /* current capacity of cache */
    static size_t cache_size = 0;    /* number of elements currently stored */

    /* small integers for the recurrence */
    static const int periodic[12] = {1,5,2,2,1,13,1,2,2,5,1,56};

    /* trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* if cache is not yet allocated allocate a default capacity */
    if (cache == NULL) {
        cache_cap  = n + 1;
        if (cache_cap < 16)
            cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)   /* out of memory */
            return (seq_t)0;
        /* initialise first two terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* extend capacity if needed */
    if (cache_size <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)    /* out of memory */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        int a_i = (i == 0) ? 28 : periodic[(i - 1) % 12];
        /* recurrence: Q_i = a_i * Q_{i-1} + Q_{i-2} */
        seq_t q = cache[i - 1] * (seq_t)a_i + cache[i - 2];
        cache[i] = q;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
