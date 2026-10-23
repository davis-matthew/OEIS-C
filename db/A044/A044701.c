/*
 * A044701  Numbers n such that string 7,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   base9_has_76
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A044701: numbers n whose base‑9 representation
   contains the substring "76" while n+1 does not.  The function
   searches numbers incrementally and memoizes the results to answer
   any call order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/base9_has_76.h"

/* The required sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* stored values of the sequence */
    static size_t cache_cap = 0;        /* allocated capacity of cache   */
    static size_t cache_len = 0;        /* number of valid entries       */
    static idx_t last_checked = 0;      /* last number examined          */

    if (n == 0) return (seq_t)0;        /* undefined for n<1, return 0 */

    /* Grow cache until we have at least n terms. */
    while (cache_len < n) {
        /* Advance to next candidate integer. */
        last_checked++;

        /* Check the base‑9 conditions. */
        if (base9_has_76(last_checked) && !base9_has_76(last_checked + (seq_t)1)) {
            /* Ensure enough space in cache. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                cache = (seq_t *)realloc((void*)cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            /* Store the found term. */
            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];    /* sequence is 1‑indexed */
}
