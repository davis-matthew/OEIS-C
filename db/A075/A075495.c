/*
 * A075495  Convolution of A075298 with A056594.
 * https://oeis.org/A075495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) - a(n-4) + a(n-5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=-6, a(3)=4, a(4)=7 This recurrence holds
 *            for all n >= 5
 * Range:     i8 n <= 13, i16 n <= 30, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of computed terms, growing as needed */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t cache_len = 0;   /* number of terms currently stored */

    /* initialize the cache with the first five terms if not yet done */
    if (cache_len == 0) {
        cache_len = 5;                           /* indices 0..4 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) { /* out of memory; return zero as sentinel (unlikely) */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)(-6);
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
    }

    /* if requested index already computed, return it */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* grow the cache to include index n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* realloc failed: memory exhausted – return 0 */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute missing terms iteratively using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) continue; /* already initialized */
        /* recurrence requires terms i-1, i-2, i-4, i-5 */
        seq_t v1 = cache[(size_t)(i - 1)];
        seq_t v2 = cache[(size_t)(i - 2)];
        seq_t v4 = (i >= 4) ? cache[(size_t)(i - 4)] : 0;
        seq_t v5 = (i >= 5) ? cache[(size_t)(i - 5)] : 0;
        cache[(size_t)i] = -v1 - (seq_t)2 * v2 - v4 + v5;
    }
    cache_len = new_len;

    return cache[(size_t)n];
}
