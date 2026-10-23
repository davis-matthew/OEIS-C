/*
 * A126627  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks digits 1,2,3 and at
 *          least one of digits 4,5,6,7,8,9.
 * https://oeis.org/A126627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*6^n-15*5^n+20*4^n-15*3^n+6*2^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the required powers */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* Positive terms */
    seq_t term1 = (seq_t)6 * pow6;
    seq_t term2 = (seq_t)20 * pow4;
    seq_t term3 = (seq_t)6 * pow2;

    /* Negative terms plus the final -1 */
    seq_t term4 = (seq_t)15 * pow5;
    seq_t term5 = (seq_t)15 * pow3;
    seq_t sum_neg = term4 + term5 + (seq_t)1;

    /* Result: (term1 + term2 + term3) - sum_neg */
    seq_t result = term1 + term2 + term3;
    result -= sum_neg;

    return result;
}
