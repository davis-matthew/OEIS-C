/*
 * A053668  Product of digits of n^3.
 * https://oeis.org/A053668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of the decimal digits of n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2294, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n³ using seq_t arithmetic */
    seq_t tn   = (seq_t)n;
    seq_t cube = tn * tn * tn;

    /* A cube of zero has product zero */
    if (cube == 0)
        return 0;

    seq_t prod = 1;
    while (cube > 0) {
        /* Extract the least significant decimal digit */
        int d = (int)(cube % 10);
        prod *= d;
        cube /= 10;

        /* If a zero digit is encountered, the product is zero */
        if (d == 0)
            return 0;
    }
    return prod;
}
