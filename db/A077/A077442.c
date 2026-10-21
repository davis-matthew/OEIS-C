/*
 * A077442  2*a(n)^2 + 7 is a square.
 * https://oeis.org/A077442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = all y such that 2*y^2+7 is a square. Recurrence: a(0)=1,
 *            a(1)=3, a(2)=9, a(3)=19. For n >= 4, a(n) = 6*a(n-2) - a(n-4)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize the first four terms on first call */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;  /* out of memory -- should never happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)19;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* out of memory */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        /* Recurrence: a(i) = 6*a(i-2) - a(i-4)  (valid for i >= 4) */
        cache[i] = (seq_t)6 * cache[i - 2] - cache[i - 4];
    }
    cache_len = new_len;

    return cache[n];
}
