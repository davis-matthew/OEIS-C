/*
 * A039834  a(n+2) = -a(n+1) + a(n) (signed Fibonacci numbers) with a(-2) =
 *          a(-1) = 1; or Fibonacci numbers (A000045) extended to negative
 *          indices.
 * https://oeis.org/A039834
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = -a(n-1) + a(n-2); initial values: a(-2) = 1, a(-1) = 1
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache variables */
    static seq_t *cache      = NULL;
    static idx_t max_computed = -3;  /* No value has been computed yet */
    static size_t capacity;         /* Allocated array length */
    const idx_t start_index = -2;   /* Index of cache[0] */

    /* Allocate and initialise cache for the first two terms if necessary */
    if (!cache) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();             /* allocation failure */
        cache[0] = (seq_t)1;             /* a(-2) */
        cache[1] = (seq_t)1;             /* a(-1) */
        max_computed = -1;               /* last computed index */
    }

    /* If the requested index has already been computed, return it */
    if (n <= max_computed)
        return cache[(size_t)(n - start_index)];

    /* Grow the cache if required */
    size_t required = (size_t)(n - start_index + 1);
    if (required > capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        capacity = required;
    }

    /* Compute missing terms up to n using the recurrence:
       a(i) = -a(i-1) + a(i-2)   for i > max_computed  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t pos_i   = i - start_index;
        idx_t pos_i1  = (i - 1) - start_index;
        idx_t pos_i2  = (i - 2) - start_index;

        seq_t v1 = cache[(size_t)pos_i1];     /* a(i-1) */
        seq_t v2 = cache[(size_t)pos_i2];     /* a(i-2) */

        cache[(size_t)pos_i] = -v1 + v2;      /* a(i) = -a(i-1) + a(i-2) */
    }

    max_computed = n;
    return cache[(size_t)(n - start_index)];
}
