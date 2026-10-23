/*
 * A190962  a(n) = 3*a(n-1) - 8*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 8*a(n-2) initial values: a(0) = 0, a(1) = 1
 *            recurrence unused for n < 2
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 22, i64 n <= 43, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;   /* number of terms already stored (size of cache) */
    static size_t capacity = 0;   /* allocated array size */

    /* initialise cache on first call */
    if (!cache)
    {
        capacity  = 2;
        cache     = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* graceful failure if out of memory */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 2;
    }

    /* if requested term already computed, return it */
    if (n < computed)
        return cache[n];

    /* grow array if necessary */
    if (n >= capacity)
    {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* graceful failure if out of memory */
        cache     = tmp;
        capacity  = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i)
    {
        seq_t val = (seq_t)3 * cache[i - 1] - (seq_t)8 * cache[i - 2];
        cache[i] = val;
    }
    computed = n + 1;

    return cache[n];
}
