/*
 * A043163  Numbers k such that 1 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_1_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043163: a(n) = the n-th integer k such that digits 1 and 6 are
   adjacent in the base‑8 representation of k but not adjacent in the
   representation of k-1.
   The function computes the terms by incremental search, storing
   previously found terms for fast future access. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_1_6.h"

/* Append a value to the dynamic cache, resizing it if necessary. */
static void append_to_cache(seq_t **c, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*c, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *c   = tmp;
        *cap = new_cap;
    }
    (*c)[*len] = val;
    ++(*len);
}

/* a(idx_t n) – returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; n==0 is out of range, return 0 for safety. */
    if (n == 0) return 0;

    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;   /* dynamic array of terms   */
    static size_t cache_len = 0;  /* number of terms stored   */
    static size_t cache_cap = 0;  /* current capacity of cache*/
    static seq_t next_candidate = 1; /* next integer to explore */

    /* If requested term already cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t k = next_candidate;
        if (has_adjacent_1_6(k) && !has_adjacent_1_6(k - 1))
            append_to_cache(&cache, &cache_len, &cache_cap, k);
        ++next_candidate;
    }

    return cache[n - 1];
}
