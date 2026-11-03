/*
 * A038125  a(n) = Sum_{k=0..n} (k-n)^k.
 * https://oeis.org/A038125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (k-n)^k
 * Range:     i8 n <= 10, i16 n <= 14, i32 n <= 22, i64 n <= 32, i128 n <= 51,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 491 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Computes the OEIS sequence A038125 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Extend cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = seq_realloc(cache, cache_size, new_size);

        /* Compute sequence values that are not yet cached */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t base = (seq_t)k - (seq_t)i;      /* (k - i) */
                seq_t term = ipow_idx(base, k);          /* (k-i)^k  */
                sum = sum + term;
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
