/*
 * A052064  Cubes containing no palindromic substring except single digits.
 * https://oeis.org/A052064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 279, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   cube_has_no_pal_subseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: cubes containing no palindromic substring except single digits
   (OEIS A052064).  The function returns the n‑th cube that satisfies this
   property, with a(1)=0.  The search is incremental and cached so that
   later calls are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cube_has_no_pal_subseq.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* static cache shared between calls */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_base  = 0;

    while (cache_len < n) {
        seq_t base  = (seq_t)next_base;
        seq_t cube = base * base * base;

        if (cube_has_no_pal_subseq(cube)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache       = newp;
                cache_cap   = new_cap;
            }
            cache[cache_len++] = cube;
        }
        ++next_base;
    }
    return cache[n - 1];
}
