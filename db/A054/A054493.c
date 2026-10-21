/*
 * A054493  A Pellian-related recursive sequence.
 * https://oeis.org/A054493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2) + 2, a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS sequence A054493 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;

    /* initial terms */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    /* initialise cache if needed */
    if (cache == NULL) {
        /* start with capacity 4 */
        cached_cap = 4;
        cached_len = 2; /* indices 0 and 1 are defined */
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* OOM: return zero as fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
    }

    /* grow cache to accommodate index n */
    if (n >= cached_cap) {
        size_t new_cap = cached_cap ? cached_cap << 1 : 4;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* OOM fallback */
        /* initialise new slots to zero (optional) */
        for (size_t i = cached_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cached_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= cached_len) {
        for (size_t i = cached_len; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i-1] - cache[i-2] + (seq_t)2;
        }
        cached_len = n + 1;
    }

    return cache[n];
}
