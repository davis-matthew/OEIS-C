/*
 * A035775  Coordination sequence for C_38 lattice.
 * https://oeis.org/A035775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)]((1+x)/(1-x))^38
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* m is the exponent 2n */
    idx_t m = (idx_t)n * 2;
    seq_t sum = 0;

    /* The outer sum runs over i = 0 .. min(38, m) */
    unsigned int max_i = (m < 38) ? (unsigned int)m : 38;
    for (unsigned int i = 0; i <= max_i; ++i)
    {
        seq_t c1 = binom(38, i);               /* (1+x)^38 coefficient */
        seq_t c2 = binom(37 + (m - i), 37);     /* (1-x)^(-38) coefficient */
        sum += c1 * c2;
    }
    return sum;
}
