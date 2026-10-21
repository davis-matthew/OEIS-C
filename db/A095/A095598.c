/*
 * A095598  a(n) = 104 written in base n.
 * https://oeis.org/A095598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = concatenate the decimal representations * of the digits
 *            of 104 written in base n. * For n = 1 the result is 104 ones
 *            (unary representation)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* base‑1 (unary) : 104 ones */
    if (n == 1) {
        char buf[105];          /* 104 digits + terminating NUL */
        for (int i = 0; i < 104; ++i)
            buf[i] = '1';
        buf[104] = '\0';
        return seq_from_str(buf);
    }

    /* Convert 104 to base n and collect the digits. */
    idx_t v = 104;
    idx_t digits[10];
    idx_t cnt = 0;
    while (v > 0 && cnt < (idx_t)(sizeof(digits)/sizeof(digits[0]))) {
        digits[cnt++] = v % n;
        v /= n;
    }

    /* Build the decimal string by concatenating the decimal
     * representations of each digit.  The most significant digit
     * comes first. */
    char buf[110];   /* more than enough for all cases */
    int pos = 0;
    for (idx_t i = cnt; i > 0; --i) {
        idx_t d = digits[i-1];
        if (d >= 100) {                /* three decimal digits */
            buf[pos++] = '0' + (d / 100);
            buf[pos++] = '0' + (d / 10) % 10;
            buf[pos++] = '0' + d % 10;
        } else if (d >= 10) {           /* two decimal digits */
            buf[pos++] = '0' + (d / 10);
            buf[pos++] = '0' + d % 10;
        } else {                        /* single decimal digit */
            buf[pos++] = '0' + d;
        }
    }
    buf[pos] = '\0';

    return seq_from_str(buf);
}
