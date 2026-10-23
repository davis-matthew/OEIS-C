/*
 * A036223  Expansion of 1/(1-3*x)^10; 10-fold convolution of A000244 (powers
 *          of 3).
 * https://oeis.org/A036223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * binomial(n+9, 9)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 24, u128 n <= 58,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    return pow3(n) * binomial(n + 9, 9);
}
