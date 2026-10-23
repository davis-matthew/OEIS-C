/*
 * A044599  Numbers n such that string 3,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_37
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044599: Numbers n whose base‑8
 * representation contains the substring "37" while n+1's base‑8
 * representation does not.  The function builds the sequence on
 * demand and caches all found values so that later calls are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_37.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* No value for index zero; behaviour below offset is irrelevant */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static seq_t max_checked = (seq_t)0;  /* largest integer examined so far */

    /* Grow the cache to hold at least n terms */
    if (cache_len < n) {
        /* Scan further numbers until we have n terms */
        seq_t x = max_checked + (seq_t)1;
        for (;; ++x) {
            if (has_37(x)) {
                if (!has_37(x + (seq_t)1)) {
                    /* store the new term */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = x;
                    if (cache_len >= n)
                        break;
                }
            }
        }
        max_checked = x;   /* x is the last examined number */
    }

    return cache[n - (idx_t)1];
}
