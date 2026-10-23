/*
 * A131039  Expansion of (1-x)*(2*x^2-4*x+1)/(1-2*x+5*x^2-4*x^3+x^4).
 * https://oeis.org/A131039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1)-5*a(n-2)+4*a(n-3)-a(n-4) with initial values
 *            a(0)=1, a(1)=-3, a(2)=-5, a(3)=7
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 65, i128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of computed elements (>=4) */
    static idx_t cache_cap = 0; /* allocated capacity */
    static int initialized = 0;

    /* Base cases: directly return constant terms if possible */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)-5;
    if (n == 3) return (seq_t)7;

    /* Ensure cache is initialized */
    if (!initialized) {
        cache_cap = 64; /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort(); /* simple OOM handling */
        /* Set base terms in cache */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)-5;
        cache[3] = (seq_t)7;
        cache_len = 4;
        initialized = 1;
    }

    /* Grow cache capacity if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) abort();
        for (idx_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];
        cache[i] = (seq_t)2 * v1 - (seq_t)5 * v2 + (seq_t)4 * v3 - v4;
    }
    cache_len = n + 1;

    return cache[n];
}
