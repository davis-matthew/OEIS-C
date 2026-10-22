/*
 * A005712  Coefficient of x^4 in expansion of (1+x+x^2)^n.
 * https://oeis.org/A005712
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2) + 3*binomial(n,3) + binomial(n,4)
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 450, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    return binom(n, 2) + 3 * binom(n, 3) + binom(n, 4);
}
