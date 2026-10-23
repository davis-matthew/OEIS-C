/*
 * A044886  Numbers having, in base 15, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_equal_even_odd_run_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search based implementation: a(n) is the nth positive integer whose
   base‑15 representation has equal totals of run lengths with even
   length and with odd length.  The function stores found terms in a
   static array so that calls with arbitrary n order are handled. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_even_odd_run_sums.h"

static seq_t *terms_list = NULL;   /* array of found terms */
static size_t terms_cap  = 0;      /* current capacity */
static idx_t  term_count  = 0;     /* number of terms found */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    /* Grow the list until we have at least n terms. */
    while (term_count < n) {
        seq_t cur = (term_count ? terms_list[term_count - 1] + 1 : 1);

        while (!has_equal_even_odd_run_sums(cur)) ++cur;

        /* Append cur to the terms list. */
        if (term_count == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(terms_list, new_cap * sizeof(seq_t));
            terms_list = tmp;
            terms_cap = new_cap;
        }
        terms_list[term_count++] = cur;
    }

    return terms_list[n - 1];
}
