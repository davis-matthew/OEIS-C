/*
 * A067108  a(n) = a number with 2n digits with first n digits odd and the
 *          rest even, in cyclic increasing order.
 * https://oeis.org/A067108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = concatenation of the first n digits of the odd‐digit
 *            cycle (1,3,5,7,9,1,3,…) followed by the first n digits of the
 *            even‐digit cycle (2,4,6,8,0,2,4,…), for n ≥ 1. The result is
 *            returned as seq_t
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* digit cycles */
    static const unsigned char odd_digits[5]  = {1, 3, 5, 7, 9};
    static const unsigned char even_digits[5] = {2, 4, 6, 8, 0};

    seq_t odd_part  = 0;   /* concatenated odd digits */
    seq_t even_part = 0;   /* concatenated even digits */

    idx_t i;
    for (i = 0; i < n; ++i) {
        odd_part = odd_part * 10 + odd_digits[i % 5];
    }
    for (i = 0; i < n; ++i) {
        even_part = even_part * 10 + even_digits[i % 5];
    }

    /* 10^n */
    seq_t pow10 = 1;
    for (i = 0; i < n; ++i) {
        pow10 *= 10;
    }

    return odd_part * pow10 + even_part;
}
