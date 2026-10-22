/*
 * A005719  Quadrinomial coefficients.
 * https://oeis.org/A005719
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*binomial(n,2) + 6*binomial(n,3) + 4*binomial(n,4) +
 *            binomial(n,5)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    return 2 * binom(n, 2) + 6 * binom(n, 3) + 4 * binom(n, 4) + binom(n, 5);
}
