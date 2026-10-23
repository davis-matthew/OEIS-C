/*
 * A105252  a(n) = binomial(n+5,n)*binomial(n+9,n).
 * https://oeis.org/A105252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,n) * binomial(n+9,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 79, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t b1 = binomial(n + 5, n);
    seq_t b2 = binomial(n + 9, n);
    return b1 * b2;
}
