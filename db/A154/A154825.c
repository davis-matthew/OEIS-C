/*
 * A154825  Reversion of x*(1-2*x)/(1-3*x).
 * https://oeis.org/A154825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>=1, a(n)=2*sum_{k=0}^{n-1} a(k)*a(n-1-k)-3*a(n-1)
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 24, i64 n <= 46, i128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined in the build environment. */

seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t len    = 0;   /* number of valid entries */

    /* Ensure we have room for indices up to n (inclusive). */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed */
        cache[0] = 1;          /* a(0) = 1 */
        len = 1;
    }
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to n, if necessary. */
    for (idx_t m = len; m <= n; ++m) {
        seq_t sum = 0;
        /* Compute convolution S(m-1) = sum_{k=0}^{m-1} a(k)*a(m-1-k). */
        for (idx_t k = 0; k < m; ++k) {
            sum += cache[k] * cache[m - 1 - k];
        }
        cache[m] = 2 * sum - 3 * cache[m - 1];
    }
    len = n + 1;   /* we now have entries 0..n */

    return cache[n];
}
