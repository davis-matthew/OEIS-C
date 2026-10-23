/*
 * A043149  Numbers k such that 4 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adj45
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043149: numbers k such that the base‑7 representation of k contains
   the digits 4 and 5 adjacent (in any order), but the representation of k‑1 does not.
   Computed by incremental search. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj45.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap  = 0;    /* capacity of cache */
static seq_t next_k = 1;         /* next integer to test */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0 but harmless */

    /* Grow cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        bool cond = has_adj45(next_k) && !has_adj45(next_k - (seq_t)1);
        if (cond) {
            /* Ensure capacity. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();         /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_k;
        }
        next_k++;        /* move to next integer */
    }
    return cache[n - (idx_t)1];
}
