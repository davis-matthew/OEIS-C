/*
 * A160303  Numerator of Hermite(n, 5/31).
 * https://oeis.org/A160303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=10, a(n+2)=10*a(n+1)-1922*(n+1)*a(n)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache to avoid recomputation.  Cache[0] and Cache[1] are always
       initialized to the two seed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;   /* highest index stored in cache */

    /* Base cases handled directly before any memory allocation. */
    if (n == (idx_t)0) return 1;
    if (n == (idx_t)1) return 10;

    /* Allocate cache on first use (or if capacity exceeded).  */
    if (cache == NULL) {
        cache_cap = (n + 1 > 16) ? (n + 1) : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failure */

        cache[0] = 1;
        cache[1] = 10;
        last_computed = 1;
    }
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure the base values are present if the cache was re‑allocated
       but last_computed is out of sync. */
    if (last_computed < (idx_t)1) {
        cache[0] = 1;
        cache[1] = 10;
        last_computed = 1;
    }

    /* Compute any missing terms up to n. */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term1 = 10 * cache[i - 1];
            seq_t term2 = (seq_t)1922 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term1 - term2;
        }
        last_computed = n;
    }

    return cache[n];
}
