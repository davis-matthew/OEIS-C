/*
 * A044794  Numbers n such that string 8,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the nth positive integer n such that the decimal string "81"
 *            appears in n but does not appear in n+1. (OEIS A044794)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has81
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has81.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    /* next integer to test */
    static seq_t next_candidate = (seq_t)81;

    if (n == 0)
        return (seq_t)0; /* undefined, but keep safe */

    /* Grow cache to hold n entries if needed */
    while (cache_len < n) {
        /* find next integer having "81" but not in the following integer */
        for ( ;; next_candidate++ ) {
            if (has81(next_candidate) && !has81(next_candidate + 1))
                break;
        }
        /* add to cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* out of memory */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_candidate;
        next_candidate++;
    }

    return cache[n - 1];
}
