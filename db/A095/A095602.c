/*
 * A095602  a(n) = 106 written in base n.
 * https://oeis.org/A095602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 106 written in base n (decimal output)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base‑1 case: 106 copies of '1' */
    if (n == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 106; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* Convert 106 (decimal) to base n */
    idx_t tmp = 106;
    unsigned int digits[16];
    size_t dlen = 0;
    while (tmp > 0) {
        digits[dlen++] = (unsigned int)(tmp % n);
        tmp /= n;
    }

    /* Build the decimal integer from the base‑n digits */
    seq_t r = 0;
    for (size_t i = 0; i < dlen; ++i) {
        seq_t d = (seq_t)digits[dlen - 1 - i];

        /* Number of decimal digits of d */
        unsigned int len = 0;
        seq_t copy = d;
        do {
            ++len;
            copy /= 10;
        } while (copy > 0);

        /* Compute 10^len */
        seq_t pow10 = 1;
        for (unsigned int k = 0; k < len; ++k)
            pow10 *= 10;

        r = r * pow10 + d;
    }

    return r;
}
