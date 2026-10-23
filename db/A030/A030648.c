/*
 * A030648  Dimensions of multiples of minimal representation of complex Lie
 *          algebra E6.
 * https://oeis.org/A030648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/165) * binomial(n+8,8) * binomial(n+11,8)
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 76, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c1 = binomial(n + 8, 8);
    seq_t c2 = binomial(n + 11, 8);

    /* Remove the three prime factors of 165 */
    if (c1 % 3 == 0)
        c1 /= 3;
    else
        c2 /= 3;

    if (c1 % 5 == 0)
        c1 /= 5;
    else
        c2 /= 5;

    if (c1 % 11 == 0)
        c1 /= 11;
    else
        c2 /= 11;

    return c1 * c2;
}
