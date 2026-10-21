/*
 * A095488  a(n) = 48 written in base n.
 * https://oeis.org/A095488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by writing 48 in base n and
 *            concatenating the base‑n digits as decimal digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* special case: base 1 (unary) – 48 ones */
    if (n == 1) {
        seq_t r = 0;
        for (int i = 0; i < 48; ++i) {
            r = r * 10 + (seq_t)1;
        }
        return r;
    }

    /* digits of 48 in base n (least significant first) */
    seq_t digits[64];
    size_t dcnt = 0;
    seq_t tmp = (seq_t)48;

    while (tmp > 0 && dcnt < sizeof(digits)/sizeof(digits[0])) {
        digits[dcnt++] = tmp % (seq_t)n;
        tmp = tmp / (seq_t)n;
    }

    /* build the decimal integer by concatenation */
    seq_t r = 0;
    for (int i = (int)dcnt - 1; i >= 0; --i) {
        seq_t d = digits[i];

        /* number of decimal digits of d */
        size_t len = 1;
        seq_t td = d;
        while (td >= 10) {
            td = td / 10;
            ++len;
        }

        /* 10^len */
        seq_t pow10 = 1;
        for (size_t j = 0; j < len; ++j) {
            pow10 = pow10 * 10;
        }

        r = r * pow10 + d;
    }

    return r;
}
