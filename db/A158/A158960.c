/*
 * A158960  Numerator of Hermite(n, 1/5).
 * https://oeis.org/A158960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 2*a(n-1) - 50*(n-1)*a(n-2) ; a(0)=1, a(1)=2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_max = (idx_t)-1;

/* Computes the n-th term of OEIS A158960 */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Initialise cache on first use */
    if (seq_cache == NULL) {
        cache_size = 2;
        seq_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!seq_cache) exit(1);          /* fatal error on allocation */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        cache_max = 1;
    }

    /* Return cached value if available */
    if (n <= cache_max)
        return seq_cache[n];

    /* Grow cache if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        idx_t new_size = cache_size ? cache_size : 2;
        while ((idx_t)(n + 1) > new_size)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)2 * seq_cache[i - 1]
            - (seq_t)50 * ((seq_t)(i - 1)) * seq_cache[i - 2];
        seq_cache[i] = term;
    }
    cache_max = n;
    return seq_cache[n];
}
