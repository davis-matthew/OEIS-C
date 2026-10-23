/*
 * A190717  Triplicated tetrahedral numbers A000292.
 * https://oeis.org/A190717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n/3)+3,3)
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t q = n / 3;          /* floor(n/3) */
    return binomial(q + 3, 3);
}
