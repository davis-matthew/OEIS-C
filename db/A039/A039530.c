/*
 * A039530  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 11's.
 * https://oeis.org/A039530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_3_and_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A039530: numbers whose base‑12 representation has the
   same non‑zero number of 3’s and 11’s.  The function searches integers
   in increasing order until the requested term is found.  A static cache
   stores already found terms so that calls with arbitrary n are fast. */

#include <stdlib.h>
#include "../utils/has_equal_3_and_11.h"

/* Main sequence entry. */
seq_t a(idx_t n) {
    /* A reasonably large static cache: 200 000 terms is plenty for n≤10 000. */
    #define CACHE_CAP 200000
    static seq_t cache[CACHE_CAP];
    static size_t cache_size = 0;
    static idx_t next_candidate = 1;

    /* Fill cache until it contains at least n terms. */
    while (cache_size < n) {
        /* Find the next integer that satisfies the digit condition. */
        while (!has_equal_3_and_11(next_candidate))
            ++next_candidate;

        /* Store the term. */
        if (cache_size < CACHE_CAP) {
            cache[cache_size++] = (seq_t)next_candidate;
        } else {
            /* If the static array is exhausted, stop expanding.  This
               should not happen for the ranges tested. */
        }
        ++next_candidate; /* prepare for next search */
    }
    return cache[n - 1];
}
