/*
 * A122598  a(0) = 0; a(1) = 1; if n is odd then a(n) = 2*a(n-1) -
 *          (n-1)*a(n-2) otherwise a(n) = 2*(a(n-1) - (n-2)*a(n-2)).
 * https://oeis.org/A122598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; a(1) = 1; a(n) = 2*a(n-1) - (n-1)*a(n-2) if n is odd
 *            and n > 1 a(n) = 2*(a(n-1) - (n-2)*a(n-2)) if n is even and n >
 *            1
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 32, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache   = NULL;
    static idx_t  cache_sz = 0;

    if (n < cache_sz)            /* already computed */
        return cache[n];

    /* enlarge cache to accommodate index n */
    idx_t old_sz   = cache_sz;
    idx_t new_sz   = n + 1;
    seq_t *tmp     = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (tmp == NULL) {                 /* allocation failed – return 0 */
        free(cache);
        cache = NULL;
        cache_sz = 0;
        return 0;
    }
    cache   = tmp;
    cache_sz = new_sz;

    /* initialise base cases if necessary */
    if (old_sz == 0) {
        cache[0] = (seq_t)0;
        old_sz = 1;
    }
    if (old_sz <= 1 && cache_sz > 1) {
        cache[1] = (seq_t)1;
        old_sz = 2;
    }

    /* compute values from the first missing index up to n */
    for (idx_t i = (old_sz > 1 ? old_sz : 2); i <= n; ++i) {
        if (i & 1) { /* odd i  */
            cache[i] = (seq_t)2 * cache[i-1]
                       - (seq_t)(i-1) * cache[i-2];
        } else {     /* even i */
            cache[i] = (seq_t)2 *
                       (cache[i-1] - (seq_t)(i-2) * cache[i-2]);
        }
    }

    return cache[n];
}
