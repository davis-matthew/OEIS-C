/*
 * A062267  Row sums of (signed) triangle A060821 (Hermite polynomials).
 * https://oeis.org/A062267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 * (a(n−1) − (n−1) * a(n−2)) (with a(0) = 1, a(1) = 2)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 17, i64 n <= 28, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static storage for values already computed */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static idx_t  count   = 0;       /* number of terms stored (next free index) */
    static idx_t  capacity = 0;      /* allocated capacity of cache */

    /* Allocate or grow cache when needed */
    if (n >= capacity)
    {
        idx_t new_cap = (capacity > 0) ? capacity : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = count; i <= n; ++i)
    {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)      cache[i] = (seq_t)2;
        else
        {
            seq_t prev1 = cache[i-1];
            seq_t prev2 = cache[i-2];
            seq_t mult  = (seq_t)(i-1);
            cache[i] = (seq_t)2 * (prev1 - mult * prev2);
        }
    }

    count = n + 1;   /* now indices 0..n are initialized */
    return cache[n];
}
