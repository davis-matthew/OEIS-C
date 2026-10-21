/*
 * A075418  Sum of generalized tribonacci numbers A001644 and inverted
 *          tribonacci numbers A075298.
 * https://oeis.org/A075418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + a(n-4) - a(n-6) a(0)=4, a(1)=2,
 *            a(2)=-2, a(3)=12, a(4)=12, a(5)=10
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 35, i64 n <= 71, i128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..5 */
    static const seq_t init_vals[6] = {4, 2, -2, 12, 12, 10};

    /* Cache of computed values.  The array is grown only when a larger
       index is requested.  The variable `max_computed` keeps the largest
       index that has already been calculated (inclusive). */
    static seq_t *cache = NULL;
    static idx_t max_computed = (idx_t)(-1);  /* uninitialized sentinel */

    /* First call: allocate cache for the first 6 elements. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        max_computed = 5;  /* we have values up to index 5 */
    }

    /* If the requested index is already computed, return it directly. */
    if (n <= max_computed)
        return cache[n];

    /* Grow the cache to accommodate all indices up to n. */
    size_t new_size = (size_t)(n + 1);       /* array size needed */
    size_t cur_size = (size_t)(max_computed + 1);
    if (new_size > cur_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) /* allocation failure: abort */
            abort();
        cache = tmp;
        cur_size = new_size;
    }

    /* Compute values from max_computed+1 up to n inclusive. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i < 6 the values are already set. */
        if (i < 6) {
            cache[i] = init_vals[i];
            continue;
        }
        /* Recurrence: a(i) = a(i-2) + 4*a(i-3) + a(i-4) - a(i-6) */
        seq_t term = cache[i - 2];
        term += (seq_t)4 * cache[i - 3];
        term += cache[i - 4];
        term -= cache[i - 6];
        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}
