/*
 * A199031  a(n) = 2*12^n-1.
 * https://oeis.org/A199031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*12^n-1 * Recurrence: a(0)=1, a(n)=12*a(n-1)+11
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
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

    /* initialise base case a(0)=1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
    }

    /* grow cache up to the requested index */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < new_len; ++i) {
            cache[i] = cache[i - 1] * (seq_t)12 + (seq_t)11;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
