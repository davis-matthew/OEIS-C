/*
 * A025737  Index of 9^n within the sequence of the numbers of the form
 *          7^i*9^j.
 * https://oeis.org/A025737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log_7(9)) (recurrence: a(0) =
 *            1, a(n) = a(n‑1) + 1 + floor(n * log_7(9)) for n ≥ 1 )
 * Range:     u8 n <= 20, u16 n <= 339, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_k_log_ratio
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/floor_k_log_ratio.h"

/* Memoised sequence a(n).  Cache is grown only when a larger
   index is requested. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache for n = 0 */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Grow cache to accommodate the requested index */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Compute values for the new indices */
        for (idx_t i = cache_size; i < new_size; ++i) {
            /* ceil(i * c) = floor(i * c) + 1 for i > 0,
               and for i == 0 the add is 1 (since a(0) = 1). */
            uint64_t add = floor_k_log_ratio(i) + 1;
            cache[i] = cache[i - 1] + (seq_t)add;
        }
        cache_size = new_size;
    }

    return cache[n];
}
