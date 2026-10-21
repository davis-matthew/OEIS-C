/*
 * A084561  Numbers with a square number of 1's in their binary expansion.
 * https://oeis.org/A084561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n‑th nonnegative integer whose binary representation
 *            contains a perfect square number of 1's
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/popcount_is_square.h"

seq_t a(idx_t n)
{
    /* cache of found terms */
    static seq_t *terms   = NULL;           /* array of terms */
    static size_t terms_len = 0;            /* number of terms stored */
    static size_t terms_cap = 0;            /* capacity of the array */
    static seq_t next_check = 0;            /* next integer to test */

    /* extend cache until it contains at least n terms */
    while (terms_len < (size_t)n) {
        /* find the next number whose popcount is a perfect square */
        while (true) {
            seq_t v = next_check++;
            if (popcount_is_square(v)) {
                /* make sure there is room in the array */
                if (terms_len + 1 > terms_cap) {
                    size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);          /* allocation failure */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = v;
                break;
            }
        }
    }

    return terms[n - 1];      /* a(1) corresponds to terms[0] */
}
