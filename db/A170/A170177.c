/*
 * A170177  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22·∑_{k=1}^{38} a(n-k) + 253·a(n-39) (n ≥ 39), a(0)=1,
 *            a(1)=24, a(n)=24·23^(n‑1) (2 ≤ n ≤ 38)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */

    /* if the requested n is greater than or equal to the current cache size,
       grow the cache and compute the missing entries */
    if (n >= cache_len) {
        idx_t new_len = n + 1;   /* new cache size (include index n) */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */

        /* initialise all new entries in turn */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                new_cache[i] = (seq_t)1;                     /* a(0) = 1 */
            } else if (i == 1) {
                new_cache[i] = (seq_t)24;                    /* a(1) = 24 */
            } else if (i <= 38) {
                /* geometric growth: a(i) = a(i‑1)·23 */
                new_cache[i] = new_cache[i-1] * (seq_t)23;
            } else {
                /* recurrence with 39‑term memory */
                seq_t sum = (seq_t)253 * new_cache[i-39];
                for (idx_t k = 1; k <= 38; ++k) {
                    sum += (seq_t)22 * new_cache[i-k];
                }
                new_cache[i] = sum;
            }
        }

        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
