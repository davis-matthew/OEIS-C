/*
 * A162759  a(n) = 16*a(n-1)-61*a(n-2) for n > 1; a(0) = 1, a(1) = 9.
 * https://oeis.org/A162759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 61*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* initialise cache with the two known terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        len   = 2;
    }

    /* if the requested term is already computed, return it */
    if ((size_t)n < len) {
        return cache[(size_t)n];
    }

    /* grow the cache so it has room for indices up to n */
    size_t target = (size_t)n + 1;
    if (target > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap < target) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = len; i < target; ++i) {
        seq_t term1 = (seq_t)16 * cache[i-1];
        seq_t term2 = (seq_t)61 * cache[i-2];
        cache[i] = term1 - term2;
    }
    len = target;

    return cache[(size_t)n];
}
