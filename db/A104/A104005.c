/*
 * A104005  a(n+3) = a(n+2) + 3a(n+1) - 2a(n); a(0) = 1, a(1) = -1, a(2)= 3.
 * https://oeis.org/A104005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+3) = a(n+2) + 3*a(n+1) - 2*a(n), a(0)=1, a(1)=-1, a(2)=3
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 33, i128 n <= 33,
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
    static idx_t cache_size = 0;

    /* initialise the first three values */
    if (!cache) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)3;
    }

    /* if the requested index is already cached, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache up to n, computing the sequence values iteratively */
    idx_t new_size = n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory */
        cache = tmp;
        /* compute new terms */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 2]
                                     - (seq_t)2 * cache[i - 3];
        }
        cache_size = new_size;
    }

    return cache[n];
}
