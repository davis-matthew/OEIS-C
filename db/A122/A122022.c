/*
 * A122022  a(n) = a(n-1) - (n-1)*a(n-4), with a(0) = 0, a(1) = 1, a(2) = 2,
 *          a(3) = 1.
 * https://oeis.org/A122022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - (n-1)*a(n-4), a(0)=0, a(1)=1, a(2)=2, a(3)=1
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 29, i64 n <= 51, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* capacity of cache */
    static size_t computed = 0;    /* number of terms computed (next index to compute) */

    /* initialise for first call */
    if (!cache) {
        cache_len = 4;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;      /* should never happen */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        computed = 4;              /* indices 0..3 are ready */
    }

    if (n < computed) /* already known */
        return cache[n];

    /* grow cache if needed */
    if ((idx_t)cache_len <= n) {
        size_t new_len = cache_len;
        while ((idx_t)new_len <= n) new_len <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure unlikely */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t mult = (seq_t)(i - 1) * cache[i - 4];
        cache[i] = cache[i - 1] - mult;
    }
    computed = n + 1;   /* terms up to n are now ready */

    return cache[n];
}
