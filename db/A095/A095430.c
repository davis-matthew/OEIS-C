/*
 * A095430  a(n) = 15 written in base n.
 * https://oeis.org/A095430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the integer formed by writing the number 15 in base n and
 *            then interpreting that base‑n representation as a decimal number
 * Range:     u8 none, u16 none, u32 none, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* base‑1 is unary: 15 ones */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 15; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* For bases > 15 the representation of 15 is a single digit 15.
       That digit is already a decimal number, so the answer is simply 15. */
    if (n > 15) {
        return (seq_t)15;
    }

    /* Convert 15 to base n (2 ≤ n ≤ 15) and build the resulting decimal integer. */
    seq_t temp = 15;
    seq_t digits[4];          /* maximum 4 digits when n = 2 (1111₍₂₎)          */
    idx_t   count = 0;

    while (temp > 0) {
        digits[count++] = temp % n;
        temp /= n;
    }

    seq_t res = 0;
    for (idx_t i = count; i > 0; --i) {
        seq_t d = digits[i - 1];
        if (d < 10) {
            res = res * (seq_t)10 + d;
        } else {                 /* d can be 10–15, represented by two decimal digits */
            res = res * (seq_t)100 + d;
        }
    }

    return res;
}
