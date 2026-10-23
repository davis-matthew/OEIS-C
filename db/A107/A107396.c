/*
 * A107396  a(n) = binomial(n+5, 5) * binomial(n+7, 5).
 * https://oeis.org/A107396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5) * binomial(n+7,5)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 20, u64 n <= 216, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return binomial(n + 5, 5) * binomial(n + 7, 5);
}
