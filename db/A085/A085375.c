/*
 * A085375  a(n) = binomial(2*n+1, n+1)*binomial(n+4, 4).
 * https://oeis.org/A085375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n + 1, n + 1) * binomial(n + 4, 4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return binomial(2 * n + 1, n + 1) * binomial(n + 4, 4);
}
