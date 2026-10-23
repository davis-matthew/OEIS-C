/*
 * A044237  Numbers k such that string 6,2 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has62_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044237: numbers k such that the base‑8 representation contains
 * the substring "62" but the representation of k-1 does not.
 * Computed by sequential search with memoisation. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has62_oct.h"

seq_t a(idx_t n)
{
    /* memoised cache of found terms */
    static seq_t *terms     = NULL;
    static size_t term_len  = 0;
    static size_t term_cap  = 0;
    static seq_t next_candidate = 1;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    if (n == 0) /* undefined, return 0 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (term_len < (size_t)n) {
        seq_t cur = next_candidate;

        if (has62_oct(cur)) {
            seq_t prev = cur - 1;
            if (!has62_oct(prev)) {
                if (term_len == term_cap) {
                    size_t new_cap = term_cap ? term_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failure */
                        exit(EXIT_FAILURE);
                    }
                    terms = tmp;
                    term_cap = new_cap;
                }
                terms[term_len++] = cur;
            }
        }

        if (next_candidate == 0) /* overflow of seq_t */
            break;
        next_candidate++;
    }

    return (term_len >= (size_t)n) ? terms[n - 1] : (seq_t)0;
}
