/*
 * A040163  a(n) is the absolute value of (the first digit of n minus the last
 *          digit of n).
 * https://oeis.org/A040163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |firstdigit(n) - lastdigit(n)|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract last digit */
    idx_t last = n % 10;

    /* Extract first digit */
    idx_t first = n;
    while (first >= 10) {
        first /= 10;
    }

    /* Compute absolute difference */
    int diff = (int)first - (int)last;
    if (diff < 0) {
        diff = -diff;
    }

    return (seq_t)diff;
}
