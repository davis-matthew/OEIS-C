/*
 * A044777  Numbers n such that string 6,4 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044777: numbers n such that decimal representation contains
   "64" but n+1 does not. Computation by a linear search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains64.h"

seq_t a(idx_t n)
{
    /* Cache of already found terms.  The cache is expanded lazily so that
       calls are independent of the order of n. */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* Grow the cache until we have at least n terms. */
    while (cache_cnt < n) {
        seq_t cur = next_candidate;
        if (contains64(cur) && !contains64(cur + 1)) {
            if (cache_cnt >= cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed – abort */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = cur;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
