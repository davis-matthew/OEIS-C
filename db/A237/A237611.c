/*
 * A237611  Squarefree numbers of form 16*k^4 + 40*k^3 + 33*k^2 + 12*k + 2,
 *          k>0.
 * https://oeis.org/A237611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237611: squarefree numbers of the form 16*k^4 + 40*k^3 + 33*k^2 + 12*k + 2,
   computed by scanning k and testing each value for squarefreeness. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Static cache of found squarefree values. */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0, cache_cap = 0;
    /* Next value of k to test. */
    static idx_t next_k = 1;

    /* Ensure the cache contains at least n terms. */
    while (cache_cnt < n) {
        seq_t k   = (seq_t)next_k;
        seq_t k2  = k * k;
        seq_t k3  = k2 * k;
        seq_t k4  = k2 * k2;
        seq_t val = 16 * k4 + 40 * k3 + 33 * k2 + 12 * k + 2;

        if (is_squarefree(val)) {
            /* Grow cache if necessary. */
            if (cache_cnt == cache_cap) {
                cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – reset cache */
                    cache = NULL;
                    cache_cnt = 0;
                    cache_cap = 0;
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_cnt++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
