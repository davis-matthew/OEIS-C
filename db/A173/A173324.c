/*
 * A173324  a(n) = 3*n! + 1.
 * https://oeis.org/A173324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n! + 1 Recurrence: a(0) = 4, a(n) = n*a(n-1) - n + 1 (n
 *            > 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache for n = 0 if needed */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)4;            /* a(0) */
    }

    /* if value already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold value for n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                  /* allocation failure – unlikely */
    cache = tmp;

    /* compute new values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v = (seq_t)i * cache[i - 1]; /* i*a(i-1) */
        v -= (seq_t)i;                    /* minus i */
        v += (seq_t)1;                    /* plus 1 */
        cache[i] = v;
    }
    cache_len = new_len;
    return cache[n];
}
