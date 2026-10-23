/*
 * A044319  Numbers n such that string 7,5 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_75
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑9 representation of n contains
 * the substring "75" but the base‑9 representation of n-1 does not.
 *
 * Implementation: iteratively scan integers, caching the results.
 * Uses a helper that tests for the substring using a base‑9 digit
 * extraction.  The cache is kept between calls so that a(n) is O(1)
 * after the required term has been computed.  The implementation
 * is valid C11 and C++17 and works for arbitrary‑precision seq_t
 * (including the big‑integer build) as well as for fixed‑width integer
 * builds.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_75.h"

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;    /* cached terms */
    static size_t terms_len = 0;   /* number of cached terms */
    static size_t terms_cap = 0;   /* allocated capacity */
    static seq_t next_candidate = (seq_t)1; /* next integer to test */

    /* Make sure we have at least n terms cached. */
    while (terms_len < n) {
        /* Find the next candidate that satisfies the definition. */
        while (!(has_75(next_candidate) &&
                 !has_75(next_candidate - (seq_t)1))) {
            ++next_candidate;
        }

        /* Grow the cache if needed. */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) {
                abort(); /* Allocation failed: caller should not rely on this. */
            }
            terms = tmp;
            terms_cap = new_cap;
        }

        /* Store the term and advance. */
        terms[terms_len++] = next_candidate;
        ++next_candidate;
    }

    return terms[n - 1];
}
