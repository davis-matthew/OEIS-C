/*
 * A045821  Numerical distance between m-th and (n+m)-th circles in a
 *          loxodromic sequence of circles in which each 4 consecutive circles
 *          touch.
 * https://oeis.org/A045821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(a(n-1)+a(n-2)+a(n-3)) - a(n-4) Initial values: a(0) =
 *            -1, a(1) = 1, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 22, i64 n <= 43, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoized values */
    static seq_t *cache      = NULL;
    static idx_t  capacity   = 0;
    static idx_t  last_computed = 0;   /* up to which index is already computed */

    /* Initialize cache with first four terms */
    if (capacity == 0) {
        capacity = 4;
        cache = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)(-1);
        cache[1] = (seq_t)(1);
        cache[2] = (seq_t)(1);
        cache[3] = (seq_t)(1);
        last_computed = 3;
    }

    /* Grow cache if needed */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
            cache[i] = ((seq_t)2 * sum) - cache[i-4];
        }
        last_computed = n;
    }

    return cache[n];
}
