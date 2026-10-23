/*
 * A169234  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 29 * 28^(n-1) for 1 ≤ n ≤ 27; a(n) = 28 *
 *            a(n-1) - 406 * a(n-28) for n ≥ 28 (recurrence is derived from
 *            the generating function of A169234)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to allow arbitrary call order. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Extend cache if necessary. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1); /* allocation failure */
        cache = new_cache;

        /* Initialise first element if the cache is empty. */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache_size = 1;
        }

        /* Compute values that are not already computed. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)29;
            } else if (i <= 27) {
                cache[i] = cache[i - 1] * (seq_t)28;
            } else {
                cache[i] = (seq_t)28 * cache[i - 1] - (seq_t)406 * cache[i - 28];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
