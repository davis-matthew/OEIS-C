/*
 * A044492  Numbers n such that string 1,3 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation for OEIS A044492
   – A number n is a member iff base‑6 representation of n
   contains the substring “13” while base‑6 of n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has13.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;
    static size_t   cache_len = 0;
    static seq_t    candidate = 1;   /* next integer to test */

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t cur = candidate;
        candidate++;

        if (has13(cur) && !has13(cur + 1)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                /* cast realloc result to seq_t* (required in C++) */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur;
        }
    }
    return cache[(size_t)(n - 1)];
}
