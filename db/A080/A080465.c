/*
 * A080465  Absolute difference between the two numbers formed by alternate
 *          digits of n.
 * https://oeis.org/A080465
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = | number formed by digits at odd positions * - number
 *            formed by digits at even positions | * * The sequence is defined
 *            for n ≥ 10, where odd positions are counted * from the most
 *            significant digit (position 1). For n < 10 the * function
 *            returns 0 (undefined behaviour is not required)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 10 the sequence is undefined; return 0. */
    if (n < 10u)
        return (seq_t)0;

    /* Extract decimal digits of n into an array (least significant first). */
    uint8_t digits[20];
    idx_t tmp = n;
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 10u;      /* store lsd first */
        tmp /= 10u;
    }

    /* Construct the numbers from odd and even positions (most significant first). */
    seq_t odd = (seq_t)0;
    seq_t even = (seq_t)0;

    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        /* Position from left: (len-1-i). Even index corresponds to odd position. */
        if (((len - 1 - i) & 1u) == 0u) {
            odd = odd * (seq_t)10 + (seq_t)d;
        } else {
            even = even * (seq_t)10 + (seq_t)d;
        }
    }

    /* Return absolute difference. */
    if (odd >= even)
        return odd - even;
    else
        return even - odd;
}
