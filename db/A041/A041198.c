/*
 * A041198  Numerators of continued fraction convergents to sqrt(110).
 * https://oeis.org/A041198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 42*a(n-2) - a(n-4) with a(0)=10, a(1)=21, a(2)=430,
 *            a(3)=881
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t    cache_len = 0;
    static size_t    cache_cap = 0;

    /* if cache not initialized, create it with at least 4 slots */
    if (cache == NULL) {
        cache_cap = (n + 1 > 4 ? n + 1 : 4);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory: unlikely in tests */

        /* initial values */
        cache[0] = (seq_t)10;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)430;
        cache[3] = (seq_t)881;
        cache_len = 4;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        while (cache_cap <= (size_t)n)
            cache_cap <<= 1;           /* double capacity */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory */
    }

    /* compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        seq_t val = (seq_t)42 * cache[cache_len - 2] - cache[cache_len - 4];
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[(size_t)n];
}
