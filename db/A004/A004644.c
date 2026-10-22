/*
 * A004644  Duplicate of A000866.
 * https://oeis.org/A004644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^n written in base 5, with the base-5 digits read as a
 *            decimal number (A000866)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* base-5 digits of 2^n, least significant first */
    size_t cap = (size_t)(n / 2 + 2);
    unsigned char *d = (unsigned char *)malloc(cap);
    size_t len = 1;
    idx_t i;
    size_t j;
    seq_t r = 0;

    d[0] = 1;
    for (i = 0; i < n; i++) {
        int carry = 0;
        for (j = 0; j < len; j++) {
            int v = d[j] * 2 + carry;
            d[j] = (unsigned char)(v % 5);
            carry = v / 5;
        }
        if (carry) d[len++] = (unsigned char)carry;
    }

    for (j = len; j > 0; j--) {
        r = r * 10 + (int)d[j - 1];
    }
    free(d);
    return r;
}
