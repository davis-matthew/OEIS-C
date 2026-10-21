/*
 * A073145  a(n) = -a(n-1) - a(n-2) + a(n-3), a(0)=3, a(1)=-1, a(2)=-1.
 * https://oeis.org/A073145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) - a(n-2) + a(n-3), a(0)=3, a(1)=-1, a(2)=-1
 * Range:     i8 n <= 15, i16 n <= 32, i32 n <= 68, i64 n <= 142,
 *            i128 n <= 288, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache to remember previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-1;
        cache_size = 3;
    }

    /* if the requested index is beyond what we have, extend and fill the cache */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;           /* need indices 0 … n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                        /* allocation failure – unlikely */
        cache = tmp;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t val = -cache[i-1] - cache[i-2] + cache[i-3];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
