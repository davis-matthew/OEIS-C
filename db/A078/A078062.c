/*
 * A078062  Expansion of (1-x)/(1+2*x+2*x^3).
 * https://oeis.org/A078062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-3) Initial values: a(0)=1, a(1)=-3,
 *            a(2)=6. This recurrence holds for all n>2
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Handle first three terms directly */
    static const seq_t init[3] = { 1, -3, 6 };
    if (n < 3)
        return init[n];

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently stored (size of cache) */

    /* If cache is empty, initialize with the first three terms */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return init[0]; /* fallback: unlikely to happen */
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
    }

    /* If requested n is beyond current cache, extend it */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) return init[0]; /* fallback on allocation failure */
        cache = new_cache;

        /* Compute all missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* temp = 2 * a[i-1] + 2 * a[i-3] */
            seq_t temp = cache[(size_t)(i - 1)];
            temp += temp;                 /* times 2 */
            seq_t part = cache[(size_t)(i - 3)];
            part += part;                 /* times 2 */
            temp += part;                 /* add the two halves */
            cache[(size_t)i] = -temp;     /* apply the recurrence */
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
