/*
 * A222253  In the number n, replace all (decimal) digits '7' with '9' and
 *          vice versa.
 * https://oeis.org/A222253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) swaps the decimal digits 7 and 9 in the decimal
 *            representation of n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n == 0 is a special case – return 0 directly */
    if (n == 0)
        return (seq_t)0;

    /* Maximal number of decimal digits of a 64‑bit unsigned integer is 20 */
    unsigned char digits[21];
    size_t count = 0;
    idx_t tmp = n;

    /* Extract digits from least significant to most significant,
       mapping 7 ↔ 9 on the fly */
    while (tmp != 0) {
        unsigned char d = (unsigned char)(tmp % 10);
        if (d == 7)
            d = 9;
        else if (d == 9)
            d = 7;
        digits[count++] = d;
        tmp /= 10;
    }

    /* Reconstruct the number in correct order */
    seq_t result = 0;
    for (size_t i = count; i > 0; --i)
        result = result * 10 + digits[i - 1];

    return result;
}
