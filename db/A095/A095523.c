/*
 * A095523  a(n) = 65 written in base 10 - n.
 * https://oeis.org/A095523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal digits of the representation of 65 in base (10-n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For 0 <= n <= 8: convert 65 to base B = 10-n and return the digits as a decimal integer. */
/* For n = 9: base 1 (unary) – return 65 ones. */
/* For n > 9: base < 1 – undefined, return 0. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute base */
    idx_t B = (idx_t)10 - n;
    if (B == 1) {
        /* Unary representation: 65 ones */
        seq_t res = 0;
        for (int i = 0; i < 65; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }
    if (B <= 0) {
        /* Undefined for base < 1 */
        return 0;
    }
    /* Convert 65 to base B */
    const int value = 65;
    int digits[16];   /* plenty of space */
    int len = 0;
    int v = value;
    while (v > 0) {
        digits[len++] = v % (int)B;
        v /= (int)B;
    }
    /* digits[0] is least significant */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
