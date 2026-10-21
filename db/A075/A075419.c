/*
 * A075419  Convolution of A073145 with A056594.
 * https://oeis.org/A075419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 31, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = -a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)
 * Initial values: a(0)=3, a(1)=-1, a(2)=-4, a(3)=6, a(4)=-1
 * This implementation memoises the sequence on demand.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first five terms on first call */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-4;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)-1;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow the cache to accommodate index n */
    size_t new_len = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i >= 5 because cache_len starts at 5 after init */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = -a1 - a2 * (seq_t)2 - a4 + a5;
    }
    cache_len = new_len;

    return cache[n];
}
