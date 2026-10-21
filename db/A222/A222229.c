/*
 * A222229  In the number n, replace all (decimal) digits '2' with '5' and
 *          vice versa.
 * https://oeis.org/A222229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal digits of n with '2' and '5' swapped
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0)
        return 0;

    /* Maximum decimal digits of a 64‑bit unsigned integer is 20 */
    unsigned char digits[20];
    size_t len = 0;
    idx_t temp = n;

    /* Extract digits from least to most significant */
    while (temp > 0) {
        digits[len++] = temp % 10;
        temp /= 10;
    }

    /* Build the transformed number from most to least significant digit */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char d = digits[len - 1 - i]; /* current digit from most significant side */
        if (d == 2)
            d = 5;
        else if (d == 5)
            d = 2;
        res = res * 10 + (seq_t)d;
    }

    return res;
}
