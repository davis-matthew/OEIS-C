/*
 * A073702  a(n) = A073145(n)^2.
 * https://oeis.org/A073702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n‑2) + 6·a(n‑3) + 3·a(n‑4) + 2·a(n‑5) – a(n‑6) with
 *            a(0)=9, a(1)=1, a(2)=1, a(3)=25, a(4)=25, a(5)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 35, u64 n <= 71, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for already computed terms.  The cache is grown when
       a larger index is requested.  Because the recurrence uses only
       previous values, the computation is O(n) and all calculations are
       performed with the ordinary +, -, and * operators provided for
       the type seq_t. */
    static seq_t *cache = NULL;    /* array of terms */
    static idx_t cache_size = 0;   /* allocated size of cache */
    static idx_t max_comp = 5;     /* highest index that has been computed */

    /* Initialize cache with the initial six terms. */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {        /* allocation failed – return zero */
            return (seq_t)0;
        }
        cache[0] = (seq_t)9;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)1;
    }

    /* If the requested index is higher than any previously computed
       value, extend the cache and compute all missing terms. */
    if (n > max_comp) {
        /* Ensure the cache is large enough.  Grow in chunks of the
           current size to keep the overall cost low. */
        if (n + 1 > cache_size) {
            idx_t new_size = n + 1;
            if (new_size < 2 * cache_size) new_size = 2 * cache_size;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) {        /* reallocation failed – return zero */
                return (seq_t)0;
            }
            cache = tmp;
            cache_size = new_size;
        }

        /* Compute all terms from max_comp+1 up to n using the
           recurrence. */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t plus  = cache[i - 3] * ((seq_t)6);
            plus += cache[i - 4] * ((seq_t)3);
            plus += cache[i - 5] * ((seq_t)2);
            seq_t sub = cache[i - 2];
            sub += cache[i - 6];
            cache[i] = plus - sub;      /* final value is guaranteed
                                            non‑negative by the
                                            recurrence definition. */
        }
        max_comp = n;
    }

    return cache[n];
}
