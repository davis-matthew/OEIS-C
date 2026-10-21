/*
 * A095624  a(n) = 117 written in base n.
 * https://oeis.org/A095624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the base‑n representation of the decimal integer 117,
 *            interpreted as a decimal integer obtained by concatenating the
 *            base‑n digits (e.g. 117 base 10 = 117, 117 base 11 = 107)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10_from_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t used only in cast */
#include "../utils/pow10_from_digits.h"

seq_t a(idx_t n) {
    if (n == 1) {
        /* Unary representation: 117 ones. */
        seq_t res = 0;
        for (idx_t i = 0; i < 117; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Compute base‑n digits of 117 (least significant first). */
    int digits[5];   /* maximum 3 digits needed for base 10‑based cases,
                       but keep a few spare for safety. */
    int dcnt = 0;
    int val = 117;

    while (val > 0) {
        digits[dcnt++] = val % (int)n;   /* n fits in int for n>=2
                                            (and 117 < INT_MAX). */
        val /= (int)n;
    }
    if (dcnt == 0) {          /* n > 117: single digit 117. */
        digits[dcnt++] = 117;
    }

    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t p10 = pow10_from_digits(d);
        res = res * p10 + d;
    }
    return res;
}
