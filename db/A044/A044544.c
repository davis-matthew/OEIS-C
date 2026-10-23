/*
 * A044544  Numbers k such that string 3,4 occurs in the base 7 representation
 *          of k but not of k+1.
 * https://oeis.org/A044544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the base‑7 representation of k contains the
   substring “34” but the base‑7 representation of k+1 does not.
   The function a(n) returns the n‑th such number with offset 1. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_34.h"

static seq_t *cache     = NULL;  /* stored terms */
static idx_t cache_len  = 0;     /* number of stored terms */
static idx_t cache_cap  = 0;     /* capacity of cache */
static seq_t last_k     = 0;     /* last candidate examined */

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms */
    while (cache_len < n) {
        last_k++; /* next candidate */

        if (has_34(last_k) && !has_34(last_k + 1)) {
            /* grow array if necessary */
            if (cache_len == cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                     new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_k;
        }
    }
    return cache[n - 1];
}
