/*
 * A193925  a(n) = a(n-1)^2 - n^(n-2) + n.
 * https://oeis.org/A193925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)^2 - n^(n-2) + n Initial values: a(0) = 0, a(1) = 0
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache capacity up to n */
    if (cache_size == 0) {
        /* initialize with first two terms */
        cache_size = (idx_t)2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;  /* out of memory – unlikely */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
    }

    if (n >= cache_size) {
        /* grow cache to include n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;  /* out of memory – unlikely */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)0;
            } else {
                seq_t prev = cache[i - 1];
                seq_t pow_val = big_pow((seq_t)i, (seq_t)(i - 2));
                cache[i] = prev * prev - pow_val + (seq_t)i;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
