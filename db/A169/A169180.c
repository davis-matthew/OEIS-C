/*
 * A169180  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = 21*(a(n-1)+...+a(0)) + 2 for 1 ≤ n ≤ 26, a(n) =
 *            21*(a(n-1)+...+a(n-26)) - 231*a(n-27) for n ≥ 27 (All operations
 *            on type seq_t.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache is large enough to hold value a(n). */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) {
            /* In an embedded environment aborting is acceptable. */
            abort();
        }
        /* Copy existing values. */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* Initialize the new entries. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            if (i < 27) {
                /* Sum a(i-1) + ... + a(0) */
                for (idx_t j = 1; j <= i; ++j)
                    sum += cache[i - j];
                seq_t const_part = (i == 0) ? (seq_t)1 : (seq_t)2;
                new_cache[i] = (seq_t)21 * sum + const_part;
            } else {
                /* Sum a(i-1) + ... + a(i-26) */
                for (idx_t j = 1; j <= 26; ++j)
                    sum += cache[i - j];
                new_cache[i] = (seq_t)21 * sum - (seq_t)231 * cache[i - 27];
            }
        }
        /* Free old cache and update pointers. */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
