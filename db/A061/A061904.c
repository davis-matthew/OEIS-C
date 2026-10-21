/*
 * A061904  Numbers n such that the iterative cycle: n -> sum of digits of n^2
 *          has only one distinct element.
 * https://oeis.org/A061904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer such that digit_sum(n^2) == 1 or 9
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* static storage for memoized terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static seq_t next_start = 1;  /* next candidate to test */

    /* ensure the buffer has enough capacity */
    if (terms_len + 1 > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }

    /* expand the list until it contains at least n terms */
    while (terms_len < n) {
        seq_t cand = next_start;
        seq_t sq   = cand * cand;        /* compute n^2 */
        seq_t s    = digit_sum(sq);      /* sum of digits of n^2 */

        if (s == 1 || s == 9) {
            terms[terms_len++] = cand;  /* store valid term */
        }
        ++next_start;
    }

    return terms[n - 1];
}
