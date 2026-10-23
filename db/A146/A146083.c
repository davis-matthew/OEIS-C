/*
 * A146083  Expansion of 1/(1 - x*(1 - 11*x)).
 * https://oeis.org/A146083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 11*a(n-2); a(0) = 1, a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 36, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t last = 1;        /* highest index computed so far */

    /* Initial allocation */
    if (cache_cap == 0) {
        cache_cap = (n >= 2 ? n + 1 : 2);          /* at least space for 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last = 1;
    }

    /* Grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms only if n > last computed */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] - (seq_t)11 * cache[i - 2];
        }
        last = n;
    }

    return cache[n];
}
