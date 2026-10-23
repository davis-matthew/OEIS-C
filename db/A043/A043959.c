/*
 * A043959  Numbers k such that 6 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k whose base‑8 representation *
 *            contains the digits 6 and 7 adjacent (either order) but the
 *            base‑8 * representation of k+1 does not
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   octal_adj67
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/octal_adj67.h"

/* Simple dynamic array to cache computed terms. */
static seq_t   *cache   = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static seq_t   next_k = 0;

/* Grow the cache if necessary and append a new term. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();        /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)            /* not used for offset 1, but defined for safety */
        return 0;

    while (cache_len < n) {
        if (octal_adj67(next_k) && !octal_adj67(next_k + 1))
            cache_push(next_k);
        next_k++;
    }
    return cache[n - 1];
}
