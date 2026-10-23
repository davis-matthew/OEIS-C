/*
 * A102677  Number of digits >= 6 in decimal representation of n.
 * https://oeis.org/A102677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n that are >= 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;            // number of digits >= 6
    idx_t x = n;                // use native idx_t for digit extraction

    /* Handle the case n == 0 separately; its single digit 0 is < 6 */
    if (x == 0)
        return count;

    while (x > 0) {
        int d = x % 10;          // current least‑significant decimal digit
        if (d >= 6)
            count += (seq_t)1;
        x /= 10;
    }
    return count;
}
