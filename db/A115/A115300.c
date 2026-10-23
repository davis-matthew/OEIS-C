/*
 * A115300  Greatest digit of n * least digit of n.
 * https://oeis.org/A115300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = (greatest digit of n) * (least digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* If n is zero, the least and greatest digits are both 0. */
    idx_t x = n;
    int greatest = -1;
    int least = 10;
    do
    {
        int d = x % 10L;
        if (d > greatest) greatest = d;
        if (d < least) least = d;
        x /= 10L;
    } while (x > 0);

    return (seq_t)greatest * (seq_t)least;
}
