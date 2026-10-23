/*
 * A024646  n written in fractional base 8/4.
 * https://oeis.org/A024646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = representation of n in fractional base 8/4 (digits 0..7
 *            with weights (8/4)^i, printed in decimal)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 3071, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t cur = n;
    uint8_t digits[65]; /* enough for 64-bit n */
    int dcnt = 0;

    while (cur != 0) {
        uint8_t digit = (uint8_t)(cur % 8);   /* d_k = cur mod 8          */
        digits[dcnt++] = digit;
        cur = (cur - digit) >> 1;             /* cur = (cur - d_k) / 2     */
    }

    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        res = res * (seq_t)10 + (seq_t)digits[i];
    }
    return res;
}
