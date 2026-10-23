/*
 * A125421  Number of base 9 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*a(n-1) - a(n-2) - 7*a(n-3) for n>3 Initial values:
 *            a(0)=1, a(1)=9, a(2)=79, a(3)=681 This implements the linear
 *            recurrence from OEIS A125421
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
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
    static size_t cache_len = 0;

    /* initialise cache with first four terms if empty */
    if (!cache) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();    /* abort on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)79;
        cache[3] = (seq_t)681;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* we need to compute terms up to n */
    size_t old_len = cache_len;            /* length before resizing */
    size_t new_len = ((size_t)n) + 1;      /* required length */

    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                /* abort on realloc failure */
        cache = tmp;
        cache_len = new_len;
        /* new elements will be overwritten below */
    }

    /* compute missing terms using the recurrence */
    for (size_t i = old_len; i <= (size_t)n; ++i)
        cache[i] = 9 * cache[i-1] - cache[i-2] - 7 * cache[i-3];

    return cache[(size_t)n];
}
