/*
 * A223910  Numbers for which the maximal run of 1's in their binary
 *          representation contains even number of 1's.
 * https://oeis.org/A223910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   maxrun_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A223910: Numbers whose maximal run of 1's in binary has even length */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/maxrun_even.h"

/* Simple dynamic array for caching the sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;   /* allocated size of seq_cache  */
static size_t seq_len  = 0;   /* number of elements stored       */
static seq_t  last_val = 0;   /* last value that was examined */

/* Grow the cache to accommodate at least `min_cap` elements. */
static void grow_cache(size_t min_cap)
{
    size_t new_cap = (seq_cap == 0) ? 4 : seq_cap * 2;
    while (new_cap < min_cap) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* abort on allocation error */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/*  a(n)  – the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        seq_t cand = 0;
        if (seq_len != 0) cand = last_val + 1;   /* next candidate */

        /* find the next integer with an even maximal run of 1's */
        while (!maxrun_even(cand)) ++cand;

        /* expand cache if necessary */
        if (seq_len == seq_cap) grow_cache(seq_len + 1);

        seq_cache[seq_len++] = cand;
        last_val = cand;
    }
    return seq_cache[n - 1];
}
