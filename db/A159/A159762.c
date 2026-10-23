/*
 * A159762  Numerator of Hermite(n, 13/21).
 * https://oeis.org/A159762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 26*a(n-1) - 882*(n-1)*a(n-2) with a(0) = 1, a(1) = 26;
 *            sequence A159762
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* for malloc, realloc, free */

static seq_t *cache = NULL;   /* memoised values */
static idx_t cache_size = 0;  /* number of stored terms */

/* Compute the nth term of sequence A159762 */
seq_t a(idx_t n)
{
    /* Initialise cache to hold the first two terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache_size = 2;
    }

    /* Extend cache until it contains a(n) */
    if (n >= cache_size) {
        /* Reallocate to hold indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failed */
        cache = tmp;

        /* Compute missing values */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = 26*a(i-1) - 882*(i-1)*a(i-2)   (for i>=2) */
            seq_t term1 = cache[i - 1];
            seq_t term2 = cache[i - 2];
            seq_t prod  = (seq_t)882 * ( (seq_t)(i - 1) * term2 );
            cache[i] = (seq_t)26 * term1 - prod;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
