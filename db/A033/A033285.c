/*
 * A033285  Numbers whose maximal base-10 run length is 4.
 * https://oeis.org/A033285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_max_run_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose maximal base-10 run length is 4.
   The algorithm generates integers in increasing order,
   keeps a cache of the already produced terms, and
   returns the nth element.  The property test scans
   the decimal digits of each candidate. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_max_run_four.h"

/* External function required by the build system */
seq_t a(idx_t n) {
    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_n   = 0;      /* number of entries in cache */
    static seq_t next_candidate = 0; /* last candidate tested */

    /* Initialise cache once */
    if (cache == NULL) {
        cache_cap = 16;
        cache    = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_n < n) {
        next_candidate++;                  /* next integer to test  */
        if (has_max_run_four(next_candidate)) {
            /* Grow cache if needed */
            if (cache_n == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_n++] = next_candidate;
        }
    }
    return cache[n-1];
}
