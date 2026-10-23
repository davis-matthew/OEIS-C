/*
 * A043102  Numbers k such that 0 and 3 occur juxtaposed in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A043102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that the base‑4 representation
 *            of k has the digits 0 and 3 adjacent (in either order), but the
 *            base‑4 representation of k‑1 does not
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_adjacent_03
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_03.h"

/* Return the n‑th term of A043102 */
seq_t a(idx_t n) {
    static seq_t *terms = NULL;          /* array of found terms */
    static size_t terms_len = 0;          /* number of terms stored */
    static size_t terms_cap = 0;          /* capacity of the array */
    static seq_t last_checked = 0;        /* last integer examined */

    /* Allocate initial storage on first call */
    if (terms == NULL) {
        terms_cap = 64;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Examine the next integer */
        last_checked++;
        seq_t k = last_checked;

        if (has_adjacent_03(k)) {
            /* Ensure k‑1 does not have the pattern */
            seq_t km1 = k - (seq_t)1;
            if (!has_adjacent_03(km1)) {
                /* Store the new term, growing the array if necessary */
                if (terms_len >= terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = k;
            }
        }
    }

    return terms[n - 1];
}
