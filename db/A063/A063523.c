/*
 * A063523  a(n) = n*(8*n^2 - 5)/3.
 * https://oeis.org/A063523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(8*n^2 - 5)/3 (integer for all n ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 829, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache so that each index is computed only once
       and later calls are O(1)                          */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* enlarge the cache to hold index n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* out‑of‑memory: the build usually aborts on this */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute all missing values using the closed formula */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;
            seq_t i_sq  = i_seq * i_seq;          /* i^2          */
            seq_t tmp   = (seq_t)8 * i_sq - (seq_t)5; /* 8*i^2-5      */
            seq_t val   = i_seq * tmp / (seq_t)3;      /* i*(8*i^2-5)/3 */
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
