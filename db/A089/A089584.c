/*
 * A089584  Numbers n which are a proper multiple (>1) of A068505(n) (= n read
 *          in base m+1 where m = largest digit of n).
 * https://oeis.org/A089584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   get_digits_rev, interpret_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth integer >9, without the digit 9, for which
   if m is its largest decimal digit, the number obtained by interpreting
   it in base (m+1) divides it properly (n / y > 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits_rev.h"
#include "../utils/interpret_in_base.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but avoid underflow */

    /* Static cache of computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;
    static seq_t candidate = 10;   /* start searching from 10 */

    /* Ensure cache growth. */
    if (terms_cnt < (size_t)n) {
        /* Grow cache if needed. */
        if (terms_cap == 0) {
            terms_cap = 32;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        }
        /* Search until we have enough terms. */
        unsigned char digits[64];
        size_t digits_len;
        unsigned char max_digit;
        while (terms_cnt < (size_t)n) {
            /* Skip any number containing digit 9. */
            if (get_digits_rev(candidate, digits, &digits_len, &max_digit)) {
                candidate++;
                continue;
            }

            seq_t base = (seq_t)(max_digit + 1);
            seq_t y = interpret_in_base(digits, digits_len, base);
            if (y != 0 && candidate > y && candidate % y == 0) {
                /* Store term. */
                if (terms_cnt == terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_cnt++] = candidate;
            }
            candidate++;
        }
    }

    return terms[n - 1];
}
