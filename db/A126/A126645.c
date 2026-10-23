/*
 * A126645  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digits 1,2,3,4 and 5, at
 *          least one of digits 6,7 and at least one of digits 8,9.
 * https://oeis.org/A126645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*3^n - 4*2^n + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 3^n using big_pow */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);

    /* a(n) = 4*pow3 - 4*pow2 + 1 */
    seq_t result = (seq_t)4 * pow3;
    result = result - ((seq_t)4 * pow2);
    result = result + (seq_t)1;

    return result;
}
