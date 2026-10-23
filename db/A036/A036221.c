/*
 * A036221  Expansion of 1/(1-3*x)^8; 8-fold convolution of A000244 (powers of
 *          3).
 * https://oeis.org/A036221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * binomial(n + 7, 7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 26, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);                /* 3^n */
    seq_t b = binomial(n + 7, 7);     /* binomial(n+7, 7) */
    return p * b;                     /* product gives a(n) */
}
