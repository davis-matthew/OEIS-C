/*
 * A101814  Even Niven (or Harshad) numbers: even numbers that are divisible
 *          by the sum of their digits.
 * https://oeis.org/A101814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive even integer that is divisible by the
 *            sum of its decimal digits (even Harshad number)
 * Range:     u8 n <= 53, u16 n <= 6647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/resize_array.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static idx_t  terms_len = 0;
    static idx_t  terms_cap = 0;
    static seq_t  last_checked = 1;  /* last number examined (start from 1) */

    if (n == 0) return 0;  /* undefined below offset, but safe */

    /* generate terms until we have at least n */
    while (terms_len < n) {
        seq_t current = last_checked + 1;
        last_checked = current;

        seq_t sum = digit_sum(current);
        /* check evenness and divisibility by digit sum */
        if ((current & 1) == 0 && current % sum == 0) {
            /* grow array if needed */
            if (terms_len == terms_cap) {
                idx_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = resize_array(terms, terms_cap, new_cap);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = current;
        }
    }

    return terms[n - 1];
}
