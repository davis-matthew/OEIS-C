/*
 * A095592  a(n) = 101 written in base n.
 * https://oeis.org/A095592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = digits of 101 in base n written as a decimal integer
 *            (concatenation of the decimal representations of the base‑n
 *            digits)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Unary base‑1: 101 ones */
    if (n == 1) {
        char s[102];
        for (int i = 0; i < 101; ++i) s[i] = '1';
        s[101] = '\0';
        return seq_from_str(s);
    }

    /* Compute the digits of 101 in base n (least significant first) */
    const idx_t VALUE = 101;
    idx_t tmp = VALUE;
    idx_t digits[128]; /* max length is 7 for n = 2 */
    size_t dlen = 0;
    while (tmp > 0) {
        digits[dlen++] = tmp % n;
        tmp /= n;
    }

    /* Build the decimal integer from the most significant digit to the least */
    seq_t result = 0;
    for (size_t i = dlen; i-- > 0; ) {
        idx_t d = digits[i];

        /* Determine the number of decimal digits of d */
        size_t len = 0;
        seq_t dd = d;
        if (dd == 0) {
            len = 1;
        } else {
            while (dd > 0) {
                dd /= 10;
                ++len;
            }
        }

        result = result * pow10((idx_t)len) + d;
    }

    return result;
}
