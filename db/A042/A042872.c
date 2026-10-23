/*
 * A042872  Numerators of continued fraction convergents to sqrt(968).
 * https://oeis.org/A042872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 39202 * a(n-6) - a(n-12) * Initial values: a(0)=31,
 *            a(1)=249, a(2)=280, a(3)=1929, * a(4)=2209, a(5)=19601,
 *            a(6)=1217471, * a(7)=9759369, a(8)=10976840, a(9)=75620409, *
 *            a(10)=86597249, a(11)=768398401
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* initial 12 terms of the recurrence */
    static const seq_t init[12] = {
        (seq_t)31ULL,
        (seq_t)249ULL,
        (seq_t)280ULL,
        (seq_t)1929ULL,
        (seq_t)2209ULL,
        (seq_t)19601ULL,
        (seq_t)1217471ULL,
        (seq_t)9759369ULL,
        (seq_t)10976840ULL,
        (seq_t)75620409ULL,
        (seq_t)86597249ULL,
        (seq_t)768398401ULL
    };

    /* lazy cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* If n is within the initially known range, return directly */
    if (n < 12)
        return init[(size_t)n];

    /* Allocate and initialise cache on first use */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                 /* OOM guard */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_size = 12;
    }

    /* Grow the cache to include n */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                 /* OOM guard */
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            /* a(i) = 39202 * a(i-6) - a(i-12) */
            seq_t prod = (seq_t)39202 * cache[i - 6];
            cache[i] = prod - cache[i - 12];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
