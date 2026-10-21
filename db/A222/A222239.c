/*
 * A222239  In the number n, replace all (decimal) digits '3' with '9' and
 *          vice versa.
 * https://oeis.org/A222239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal digits of n with every digit '3' replaced by '9'
 *            and every digit '9' replaced by '3'. The digits are otherwise
 *            unchanged
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Buffer for up to 20 decimal digits (for 64‑bit n). */
    uint8_t buf[20];
    int pos = 0;
    idx_t temp = n;

    /* Extract digits from least significant to most significant,
       swapping 3 <-> 9 while storing them. */
    while (temp > 0) {
        uint8_t d = temp % 10;
        if (d == 3)
            d = 9;
        else if (d == 9)
            d = 3;
        buf[pos++] = d;
        temp /= 10;
    }

    /* Reconstruct the number in the correct digit order. */
    seq_t res = 0;
    for (int i = pos - 1; i >= 0; --i) {
        res = res * 10 + (seq_t)buf[i];
    }
    return res;
}
