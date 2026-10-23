/*
 * A017880  Expansion of 1/(1-x^9-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13); a(0)=1,
 *            a(1..8)=0, a(9..13)=1
 * Range:     u8 n <= 51, u16 n <= 91, u32 n <= 165, u64 n <= 315,
 *            u128 n <= 614, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialisation: allocate 14 slots and set the base values */
    if (cache_len == 0) {
        cache_len = 14;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = 0;
        cache[0] = 1;
        cache[9] = cache[10] = cache[11] = cache[12] = cache[13] = 1;
    }

    /* if requested index already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to accommodate the requested index */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – aborting the program; in production you
           would handle this differently, but for the exercise we simply
           exit to avoid undefined behaviour. */
        abort();
    }
    cache = tmp;

    /* initialise any newly allocated slots to 0 before using them */
    for (idx_t i = cache_len; i < new_len; ++i)
        cache[i] = 0;

    /* compute new values using the recurrence until we reach n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i-9] + cache[i-10] + cache[i-11] +
                    cache[i-12] + cache[i-13];
        cache[i] = sum;
    }

    cache_len = new_len;
    return cache[n];
}
