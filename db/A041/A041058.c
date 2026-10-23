/*
 * A041058  Numerators of continued fraction convergents to sqrt(35).
 * https://oeis.org/A041058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 12*a(n-2) - a(n-4) ; Initial values: a(0)=5, a(1)=6,
 *            a(2)=65, a(3)=71
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;                 /* dynamic array of computed terms   */
static size_t cache_cap = 0;                /* current capacity of the array     */
static size_t cache_size = 0;               /* number of terms currently stored */

static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t cap = cache_cap ? cache_cap : 4;
    while (cap < new_cap)
        cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE); /* allocation failure */
    cache = new_cache;
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache contains the first four initial values */
    if (!cache)
    {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)65;
        cache[3] = (seq_t)71;
        cache_cap = 4;
        cache_size = 4;
    }

    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    size_t target = (size_t)n;
    grow_cache(target + 1);

    for (size_t i = cache_size; i <= target; ++i)
    {
        /* a(i) = 12*a(i-2) - a(i-4) */
        cache[i] = (seq_t)12 * cache[i - 2] - cache[i - 4];
    }

    cache_size = target + 1;
    return cache[target];
}
