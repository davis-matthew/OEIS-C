/*
 * A167993  Expansion of x^2/((3*x-1)*(3*x^2-1)).
 * https://oeis.org/A167993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 9*a(n-3), a(0) = 0, a(1) = 0, a(2)
 *            = 1 (OEIS A167993)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage function required by the build system */
seq_t a(idx_t n) {
    /* Cached values of the sequence */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;       /* number of elements currently allocated */

    /* First call: create initial cache with the first three terms */
    if (cache == NULL) {
        cache_len = (n + 1 > 3 ? n + 1 : 3);   /* at least three slots */
        cache = (seq_t *)calloc((size_t)cache_len, sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failed */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        /* Compute terms up to n if needed */
        if (n >= 3) {
            for (idx_t i = 3; i <= n; ++i) {
                seq_t t1 = (seq_t)3 * cache[i - 1];
                seq_t t2 = (seq_t)3 * cache[i - 2];
                seq_t t3 = (seq_t)9 * cache[i - 3];
                cache[i] = t1 + t2 - t3;
            }
        }
        return cache[n];
    }

    /* Cache exists: check if we need to extend it */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failed */
        cache = tmp;
        /* Compute new terms from old cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = (seq_t)3 * cache[i - 1];
            seq_t t2 = (seq_t)3 * cache[i - 2];
            seq_t t3 = (seq_t)9 * cache[i - 3];
            cache[i] = t1 + t2 - t3;
        }
        cache_len = new_len;
    }

    return cache[n];
}
