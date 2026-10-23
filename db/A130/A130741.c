/*
 * A130741  a(n) = 4^(4^n) + 3.
 * https://oeis.org/A130741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(4^n) + 3; offset 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"     /* declares seq_t pow4(idx_t exp); */
#include "../utils/big_pow.h"  /* declares seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* Compute 4^n as a seq_t (the exponent for the outer power) */
    seq_t exp = pow4((idx_t)n);

    /* Compute 4^(4^n) using the arbitrary-precision power function */
    seq_t val = big_pow((seq_t)4, exp);

    /* Add 3 to get the final term */
    val += (seq_t)3;

    return val;
}
