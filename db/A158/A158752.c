/*
 * A158752  a(n) = Hermite(n, 23).
 * https://oeis.org/A158752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=46, a(n)=46*a(n-1)-2*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;        /* highest index that has been computed */

    /* initialise cache with the first two terms */
    if (!cache) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory - unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)46;
        max_computed = 1;
    }

    /* grow the cache if the requested index is beyond current capacity */
    if (cap <= (size_t)n) {
        size_t newcap = cap << 1;          /* double capacity */
        if (newcap <= (size_t)n) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* if n is already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t term1 = (seq_t)46 * cache[k - 1];
        seq_t term2 = (seq_t)2 * (seq_t)(k - 1) * cache[k - 2];
        cache[k] = term1 - term2;
    }
    max_computed = n;
    return cache[n];
}
