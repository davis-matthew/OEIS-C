/*
 * A052425  Numbers with no two-digit factors (apart from itself).
 * https://oeis.org/A052425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_two_digit_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers with no two‑digit proper divisor.
   The function a(idx_t n) returns the n‑th term of the sequence,
   memoising results so that calls may arrive in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_two_digit_factor.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* array holding known terms */
    static size_t terms_cap = 0;     /* total capacity of the array   */
    static size_t terms_count = 0;   /* number of terms stored        */
    static idx_t  next_candidate = 1;/* next integer to test          */

    /* Return cached value if already computed. */
    if (n <= terms_count) return terms[n - 1];

    /* Grow the array when required. */
    if (terms_cap == 0) {
        terms_cap = 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n of them. */
    while (terms_count < n)
    {
        seq_t cand = (seq_t)next_candidate;

        if (!has_two_digit_factor(cand))
        {
            /* Ensure capacity. */
            if (terms_count == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_count++] = cand;
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
