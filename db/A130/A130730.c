/*
 * A130730  Fermat numbers of order 7 or F(n,7) = 2^(2^n) + 7.
 * https://oeis.org/A130730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n) + 7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n as a seq_t exponent */
    seq_t exp = pow2(n);

    /* Compute 2^(exp) as a seq_t result */
    seq_t val = big_pow((seq_t)2, exp);

    /* Add 7 to get the Fermat number of order 7 */
    val += (seq_t)7;

    return val;
}
