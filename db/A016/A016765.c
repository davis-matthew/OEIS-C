/*
 * A016765  Expansion of g.f. 1/((1-3*x)*(1-4*x)*(1-6*x)).
 * https://oeis.org/A016765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(n+1) - 2^(2*n+3) + 3^(n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Exponent for 6 and 3: n + 1   */
    seq_t exp = (seq_t)n + 1;

    /* 6^(n+1)  */
    seq_t pow6 = big_pow(6, exp);

    /* 3^(n+1)  */
    seq_t pow3 = big_pow(3, exp);

    /* Exponent for 2: 2*n + 3 */
    seq_t exp2 = ((seq_t)n) * 2 + 3;

    /* 2^(2*n+3) */
    seq_t pow2 = big_pow(2, exp2);

    /* a(n) = 6^(n+1) - 2^(2*n+3) + 3^(n+1) */
    return pow6 - pow2 + pow3;
}
