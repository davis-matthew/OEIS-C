/*
 * A043796  Numbers whose number of runs in base 3 is congruent to 5 (mod 7).
 * https://oeis.org/A043796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   runs_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043796: Numbers whose number of runs in base 3 is congruent to 5 (mod 7). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/runs_base3.h"

/* Sequence generator. */
seq_t a(idx_t n)
{
    /* For n < 1, return 0 as a sentinel. */
    if (n < 1) return 0;

    /* Static cache of the discovered terms. */
    static seq_t *cache      = NULL;   /* array of found terms   */
    static size_t cache_cap  = 0;      /* allocated capacity    */
    static size_t cache_size = 0;      /* number of stored terms */
    static seq_t next_candidate = 1;   /* next integer to test   */

    /* Grow the cache when necessary. */
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure → sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n entries. */
    while (cache_size < (size_t)n) {
        seq_t val = next_candidate;
        if ((runs_base3(val) % 7) == 5) {
            cache[cache_size++] = val;
        }
        /* Advance to the next integer value. */
        next_candidate = val + (seq_t)1;
    }

    return cache[n - 1];  /* zero‑based index in the cache */
}
