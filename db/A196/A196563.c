/*
 * A196563  Number of even digits in decimal representation of n.
 * https://oeis.org/A196563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of even decimal digits in n (a(0)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U)            /* 0 itself is one even digit */
        return (seq_t)1;

    seq_t count = 0;
    idx_t x = n;
    while (x != 0U) {
        uint8_t d = (uint8_t)(x % 10U);
        if ((d & 1U) == 0U) /* even digit */
            count++;
        x /= 10U;
    }
    return count;
}
