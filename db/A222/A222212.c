/*
 * A222212  In the number n, replace all (decimal) digits '0' with '3' and
 *          vice versa.
 * https://oeis.org/A222212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace all decimal digits '0' with '3' and vice versa
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)3;

    /* Extract decimal digits of n into a temporary buffer (reversed order) */
    unsigned char digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Compose the result by processing digits from most significant to least */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char d = digits[len - 1 - i];
        if (d == 0)
            d = 3;
        else if (d == 3)
            d = 0;
        res = res * 10 + (seq_t)d;
    }
    return res;
}
