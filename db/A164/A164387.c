/*
 * A164387  Number of binary strings of length n with no substrings equal to
 *          0000 or 0010.
 * https://oeis.org/A164387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) + a(n-5), a(0) = 1, a(1) = 2,
 *            a(2) = 4, a(3) = 8, a(4) = 14
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 74, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values, independent of call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored (max index + 1) */

    /* initialise base values once */
    if (cache_len == 0) {
        cache_len = 5;   /* indices 0..4 */
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)14;
    }

    /* if the requested value is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold up to index n */
    idx_t old_len = cache_len;
    if ((size_t)(n + 1) > old_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_len = (idx_t)(n + 1);
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = old_len; i <= n; ++i) {
        /* for i >= 5 all required previous terms exist in the cache */
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 4] + cache[i - 5];
    }

    return cache[n];
}
