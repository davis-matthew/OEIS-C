/*
 * A062999  Numbers whose sum of the digits is strictly less than its product
 *          of digits.
 * https://oeis.org/A062999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) are the integers whose sum of digits is strictly less
   than their product of digits. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_candidate = 1;

    /* Extend cache until it contains at least n terms. */
    while (terms_len < n) {
        seq_t val = next_candidate;
        seq_t sum = 0;
        seq_t prod = 1;
        while (val > 0) {
            seq_t d = val % 10;
            sum += d;
            prod *= d;
            val /= 10;
        }

        if (prod > sum) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_candidate;
        }
        next_candidate++;
    }

    return terms[n - 1];
}
