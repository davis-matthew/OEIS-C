/*
 * A040075  5-fold convolution of A000302 (powers of 4); expansion of
 *          1/(1-4*x)^5.
 * https://oeis.org/A040075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4) * 4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 54,
 *            big any n
 * Helpers:   binom, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute (n+4 choose 4) * 4^n */
    return binom(n + 4, 4) * pow4(n);
}
