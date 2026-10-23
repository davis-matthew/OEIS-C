/*
 * A129393  Row sums of A129392.
 * https://oeis.org/A129393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 30, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -2*a(n-1) - a(n-2) + 4*a(n-3) for n>2,
   a(0) = 1, a(1) = 5, a(2) = -15.  Row sums of A129392. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)(-15);

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated entries */
    static idx_t last_computed = 2; /* highest index already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)(-15);
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* reallocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = -2 * cache[i - 1] - cache[i - 2] + 4 * cache[i - 3];
    }
    last_computed = n;

    return cache[n];
}
