/*
 * A020570  Expansion of g.f. 1/((1-6*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A020570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*6^n - 49*7^n + 32*8^n * 1/((1-6*x)*(1-7*x)*(1-8*x))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Calculate 6^n, 7^n, and 8^n using the provided big_pow helper.
       The exponent is cast to seq_t because big_pow expects a seq_t argument. */
    seq_t exp = (seq_t)n;
    seq_t pow6 = big_pow((seq_t)6, exp);
    seq_t pow7 = big_pow((seq_t)7, exp);
    seq_t pow8 = big_pow((seq_t)8, exp);

    /* Compute the linear combination: 18*6^n + 32*8^n - 49*7^n. */
    seq_t term1 = pow6 * (seq_t)18;
    seq_t term3 = pow8 * (seq_t)32;
    seq_t term2 = pow7 * (seq_t)49;

    seq_t result = term1 + term3;
    result -= term2;
    return result;
}
