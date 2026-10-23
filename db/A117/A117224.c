/*
 * A117224  Numbers for which the square and the cube have the same digital
 *          sum.
 * https://oeis.org/A117224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 138, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A117224:  
   Numbers for which the square and the cube have the same digital sum. */
/* For any n >= 1, returns the term a(n) with offset 1. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  size  = 0;
    static idx_t  next  = 0;          /* next integer to test */

    /* Lazy initialization of cache */
    if (cache == NULL) {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        size  = 0;
        next  = 0;
    }

    /* Generate terms until we reach the desired index */
    while (size < n) {
        seq_t cand   = (seq_t)next;          /* current candidate */
        seq_t sq     = cand * cand;          /* n^2 */
        seq_t cu     = sq * cand;            /* n^3 */

        seq_t sum_sq = digit_sum(sq);
        seq_t sum_cu = digit_sum(cu);

        if (sum_sq == sum_cu) {
            /* Ensure cache has enough capacity */
            if ((size_t)size >= cap) {
                size_t new_cap = cap * 2;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                memcpy(new_cache, cache, cap * sizeof(seq_t));
                free(cache);
                cache = new_cache;
                cap   = new_cap;
            }
            cache[size++] = cand;
        }
        next++; /* move to the next integer to test */
    }

    return cache[n - 1];
}
