/*
 * A043730  Numbers whose number of runs in their base-2 representation is
 *          congruent to 2 mod 5.
 * https://oeis.org/A043730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_runs_in_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A043730: Numbers whose number of runs in their base‑2 representation is
 *  congruent to 2 mod 5.
 *  The implementation scans successive integers, counts binary runs, and
 *  keeps those with (runs % 5) == 2.  Results are memoised so that
 *  repeated calls are very fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_runs_in_binary.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* a: returns the n‑th term of A043730 (n ≥ 1).  The sequence is memoised
 * in a static array so that subsequent calls are O(1) after the needed
 * terms have been generated. */
seq_t a(idx_t n)
{
    /* Static cache of generated terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t  size  = 0;     /* number of stored terms */
    static size_t  cap   = 0;     /* allocated capacity */
    static seq_t   next  = ((seq_t)2); /* next integer to test */

    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Grow the cache if necessary. */
        if (cap <= size) {
            size_t new_cap = cap ? cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        seq_t x = next++;             /* candidate integer            */
        if (count_runs_in_binary(x) % 5 == 2) {
            cache[size++] = x;         /* store if it satisfies property */
        }
    }

    return cache[n - 1];
}
