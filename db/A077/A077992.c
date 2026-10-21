/*
 * A077992  Expansion of 1/(1+2*x+2*x^2-x^3).
 * https://oeis.org/A077992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-2) + a(n-3) with a(0)=1, a(1)=-2,
 *            a(2)=2. Expansion of 1/(1+2*x+2*x^2-x^3)
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 42, i64 n <= 83, i128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;        /* cache of computed values */
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;

    /* Allocate cache (first time or if n exceeds current size) */
    if (cache == NULL) {
        /* Ensure at least space for the first 3 terms */
        cache_size = (n + 1 > 3 ? n + 1 : 3);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        /* Initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)2;
        max_computed = 2;
    } else if (n >= cache_size) {
        /* Grow cache to accommodate index n */
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;       /* realloc failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            cache[i] = -2 * t1 - 2 * t2 + t3;
        }
        max_computed = n;
    }

    return cache[n];
}
