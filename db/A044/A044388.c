/*
 * A044388  Numbers n such that string 5,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has56_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer in base‑10 whose representation contains the
   substring "56" while (n-1) does not. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has56_pair.h"

/* Return the nth term of A044388.  The function caches the terms
   that have been generated so far so that any order of calls works.
   The implementation scans successive integers starting from
   1, tests the property, and stores the qualifying numbers. */
seq_t a(idx_t n)
{
    static seq_t *terms      = NULL;    /* array of found terms */
    static size_t terms_cap  = 0;       /* current allocation size   */
    static size_t terms_cnt  = 0;       /* number of terms stored    */
    static seq_t next_candidate = 1;    /* next integer to test      */

    /* Ensure we have storage for at least n terms. */
    if (terms_cnt < (size_t)n) {
        if (terms_cap < (size_t)n) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            terms_cap = new_cap;
        }

        /* Scan numbers, starting from the last tested value, until
           we have identified n terms. */
        while (terms_cnt < (size_t)n) {
            if (has56_pair(next_candidate)) {
                seq_t prev = next_candidate - (seq_t)1;
                if (!has56_pair(prev))
                    terms[terms_cnt++] = next_candidate;
            }
            seq_t last = next_candidate;
            next_candidate++;
            /* Detect unsigned wrap‑around of seq_t; stop to avoid
               an infinite loop if the allocation range has been
               exhausted. */
            if (next_candidate < last)
                break;
        }
    }

    return terms[(size_t)n - 1];
}
