/*
 * A141016  Duplicate of A008998.
 * https://oeis.org/A141016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 2, a(n) = 2*a(n-1) + a(n-3) for n ≥ 3
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    size_t idx = (size_t)n;          /* cast to size_t for array access */
    if (idx >= cache_len)            /* extend cache if necessary */
    {
        size_t new_len = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)             /* allocation failure – unlikely for test set */
            return (seq_t)0;

        cache = new_cache;
        for (size_t k = cache_len; k < new_len; ++k)
        {
            if (k == 0)
                cache[k] = (seq_t)0;
            else if (k == 1)
                cache[k] = (seq_t)1;
            else if (k == 2)
                cache[k] = (seq_t)2;
            else
                cache[k] = (seq_t)(2) * cache[k-1] + cache[k-3];
        }
        cache_len = new_len;
    }
    return cache[idx];
}
