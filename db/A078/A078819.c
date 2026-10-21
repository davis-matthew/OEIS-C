/*
 * A078819  a(n) = 140*C(2n,n)/(n+4).
 * https://oeis.org/A078819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 140*C(2n,n)/(n+4)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=35, a(n)=a(n-1)*(4*n*n+10*n-6)/(n*n+4*n) */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* initialise cache with a(0) */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */
        cache[0] = (seq_t)35;
        cache_size = 1;
    }

    /* grow cache up to required n */
    while (cache_size <= n) {
        idx_t next_n = cache_size;          /* next index to compute */
        seq_t prev_val = cache[cache_size - 1];

        seq_t nn   = (seq_t)next_n;
        seq_t nn_sq = nn * nn;

        seq_t numerator_factor = (seq_t)4 * nn_sq + (seq_t)10 * nn - (seq_t)6;
        seq_t numerator = prev_val * numerator_factor;
        seq_t denominator = nn_sq + (seq_t)4 * nn;

        seq_t next_val = numerator / denominator;  /* division is exact */

        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (new_cache == NULL) return (seq_t)0;  /* allocation failed */
            cache = new_cache;
        }
        cache[cache_size++] = next_val;
    }

    return cache[n];
}
