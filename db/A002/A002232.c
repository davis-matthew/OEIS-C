/*
 * A002232  8th powers written backwards.
 * https://oeis.org/A002232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reversal of the decimal digits of n^8; n^8 is built as a
 *            decimal digit array, so it never overflows seq_t
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 237,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned char d[200];
    int len = 0, i, k;
    idx_t m = n;
    seq_t result = 0;

    if (n == 0) return (seq_t)0;
    while (m > 0) {
        d[len++] = (unsigned char)(m % 10);
        m /= 10;
    }
    for (k = 1; k < 8; k++) {
        unsigned __int128 carry = 0;
        for (i = 0; i < len; i++) {
            unsigned __int128 t = (unsigned __int128)d[i] * n + carry;
            d[i] = (unsigned char)(t % 10);
            carry = t / 10;
        }
        while (carry > 0) {
            d[len++] = (unsigned char)(carry % 10);
            carry /= 10;
        }
    }
    /* d[0] is the least significant digit of n^8, so it becomes the most significant digit of the result */
    for (i = 0; i < len; i++)
        result = result * 10 + (seq_t)d[i];
    return result;
}
