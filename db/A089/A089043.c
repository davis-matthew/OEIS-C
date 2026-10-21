/*
 * A089043  a(n) = n!^2 + (-1)^n.
 * https://oeis.org/A089043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!)^2 + (-1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t f = fact_upto(n);
    /* square it */
    seq_t sq = f * f;
    /* add or subtract 1 depending on parity of n */
    if ((n & 1) == 0)
        return sq + 1;
    else
        return sq - 1;
}
