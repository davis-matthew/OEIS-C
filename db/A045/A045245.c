/*
 * A045245  Numbers whose base-5 representation contains exactly three 1's and
 *          one 3.
 * https://oeis.org/A045245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth number whose base‑5 representation contains
 *            exactly three 1's and exactly one 3
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_three_ones_and_one_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/has_three_ones_and_one_three.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Sanity check for offset: a(1) is the first term. */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Static storage for generated terms. */
    static seq_t *term = NULL;
    static size_t term_len = 0;
    static size_t term_cap = 0;
    static seq_t candidate = 0;   /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (term_len < (size_t)n) {
        if (has_three_ones_and_one_three(candidate)) {
            /* Grow the array if necessary. */
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(term, new_cap * sizeof(seq_t));
                term = tmp;
                term_cap = new_cap;
            }
            term[term_len++] = candidate;
        }
        candidate++;
    }

    return term[n - 1];
}
