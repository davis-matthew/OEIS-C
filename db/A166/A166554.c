/*
 * A166554  a(0)=1, a(n) = n*(a(n-1) - 1) for n>0.
 * https://oeis.org/A166554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 and a(n) = n * (a(n-1) - 1) for n > 0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache    = NULL;
    static idx_t cache_sz = 0;
    static idx_t last_computed = 0;   /* highest index whose value is cached */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_sz = 1;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        last_computed = 0;
    }

    /* grow the table if necessary */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)i * (cache[i - 1] - (seq_t)1);
    }
    last_computed = n;

    return cache[n];
}
