/*
 * A157096  Consider all consecutive integer Pythagorean 11-tuples (X, X+1,
 *          X+2, X+3, X+4, X+5, Z-4, Z-3, Z-2, Z-1, Z) ordered by increasing
 *          Z; sequence gives X values.
 * https://oeis.org/A157096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=55; a(n)=22*a(n-1)-a(n-2)+50 for n>1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static idx_t cache_limit = 0; /* highest index already computed */

    /* Allocate or expand cache if needed */
    if (cache == NULL) {
        cache_capacity = n + 1;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on allocation failure */
        cache[0] = (seq_t)0;           /* a(0) */
        cache_limit = 0;
    } else if (cache_capacity <= (size_t)n) {
        cache_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* fallback on allocation failure */
        cache = tmp;
    }

    /* Ensure base values */
    if (cache_limit < 1 && n >= 1) {
        cache[1] = (seq_t)55;        /* a(1) */
        cache_limit = 1;
    }

    /* Compute missing terms */
    if (cache_limit < n) {
        for (idx_t i = cache_limit + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* sum = 22 * cache[i-1] using repeated addition to avoid overflow */
            for (int k = 0; k < 22; ++k) sum += cache[i - 1];
            sum -= cache[i - 2];
            sum += (seq_t)50;
            cache[i] = sum;
        }
        cache_limit = n;
    }

    return cache[n];
}
