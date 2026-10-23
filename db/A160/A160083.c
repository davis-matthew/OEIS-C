/*
 * A160083  Numerator of Hermite(n, 23/26).
 * https://oeis.org/A160083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n, 23/26) Recurrence from Hermite
 *            polynomials: a_0 = 1, a_1 = 23, a_{n+1} = 23 * a_n - 338 * n *
 *            a_{n-1} (n >= 1) (since 338 = 2 * 13^2, 23 = 13 * 2 * (23/26))
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases immediately. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;       /* array of computed terms   */
    static size_t cache_cap = 0;      /* allocated capacity       */
    static idx_t cache_last = 1;      /* highest index computed  */

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap = 2;                /* space for a[0] and a[1] */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
    }

    /* If the requested term is already in the cache, return it. */
    if (n <= cache_last) {
        return cache[n];
    }

    /* Grow the cache if necessary. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_last+1 up to n inclusive. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* Recurrence: a[i] = 23 * a[i-1] - 338 * (i-1) * a[i-2] */
        seq_t prod = (seq_t)(i - 1) * cache[i - 2];
        prod = prod * (seq_t)338;
        cache[i] = (seq_t)23 * cache[i - 1] - prod;
    }
    cache_last = n;
    return cache[n];
}
