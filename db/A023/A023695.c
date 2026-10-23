/*
 * A023695  Numbers with exactly 4 1's in their ternary expansion.
 * https://oeis.org/A023695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_ones_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer whose ternary expansion
   contains exactly four digits equal to 1. */

#include <stdlib.h>
#include "../utils/count_ones_ternary.h"

/* Grow a dynamic array of seq_t values, allocating in powers of two. */
static void grow_cache(size_t needed, size_t *capacity, seq_t **array)
{
    if (needed <= *capacity)
        return;
    size_t new_cap = (*capacity) ? (*capacity) * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
    *array   = tmp;
    *capacity = new_cap;
}

/* Return the nth term of sequence A023695 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined for n < 1, but safe return. */
        return 0;

    /* Static cache to serve any calling order. */
    static seq_t *cache   = NULL; /* array of found values   */
    static size_t cache_l = 0;   /* number of values cached */
    static size_t cache_s = 0;   /* capacity of allocated array */
    static seq_t next_val   = 0; /* next integer to examine   */

    /* Generate values until we have at least n terms cached. */
    while (cache_l < n) {
        if (count_ones_ternary(next_val) == 4) {
            grow_cache(cache_l + 1, &cache_s, &cache);
            cache[cache_l++] = next_val;
        }
        ++next_val;
    }

    /* OEIS offset is 1: a(1) corresponds to cache[0]. */
    return cache[n - 1];
}
