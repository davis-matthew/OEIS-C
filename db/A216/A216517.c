/*
 * A216517  Even numbers n such that 2^n-1 can be written in the form
 *          a^2+7*b^2.
 * https://oeis.org/A216517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * 7^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* below offset, irrelevant */
    seq_t pow7 = seq_pow((seq_t)7, n - 1); /* compute 7^(n-1) */
    return (seq_t)6 * pow7;                /* multiply by 6 */
}
