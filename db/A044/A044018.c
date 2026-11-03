/*
 * A044018  Numbers k such that 2 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that 2 and 8 occur juxtaposed in the base-10 *
 *            representation of k but not of k+1. * Computation: iterative
 *            search from 1 upwards, memoising results
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether decimal representation of num contains the adjacent digit
 * pair "28" or "82". */
static bool has_adjacent_28or82(seq_t num)
{
    if (num == 0) return false;            /* 0 has no decimal digits */
    seq_t prev = 10;                       /* sentinel value larger than 9 */
    while (num) {
        seq_t cur = num % 10;
        num     /= 10;
        if (prev != 10) {
            if ((cur == 2 && prev == 8) ||
                (cur == 8 && prev == 2))
                return true;
        }
        prev = cur;
    }
    return false;
}

/* Sequence a(n) for n >= 1, 1‑based indexing. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t cache_size   = 0;   /* number of cached terms */
    static size_t cache_cap    = 0;   /* cached storage capacity */
    static idx_t  last_checked = 0;   /* last integer tested */

    /* Ensure we have enough capacity to store new terms. */
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < cache_size + 1) new_cap = cache_size + 1;
        seq_t *p = mem_resize(seq_t, cache, new_cap);
        if (!p) exit(1);   /* allocation failure */
        cache      = p;
        cache_cap  = new_cap;
    }

    /* Generate further terms until we have the nth term. */
    while (cache_size < n) {
        last_checked++;
        if (has_adjacent_28or82((seq_t)last_checked) &&
            !has_adjacent_28or82((seq_t)(last_checked + 1))) {
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
