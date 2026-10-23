/*
 * A019839  Expansion of 1/((1-5x)(1-6x)(1-10x)).
 * https://oeis.org/A019839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n - 9*6^n + 5*10^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow5  = seq_pow((seq_t)5,  n);
    seq_t pow6  = seq_pow((seq_t)6,  n);
    seq_t pow10 = seq_pow((seq_t)10, n);

    seq_t result = pow5 * 5;
    result += pow10 * 5;
    result -= pow6 * 9;
    return result;
}
