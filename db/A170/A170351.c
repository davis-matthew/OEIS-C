/*
 * A170351  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 , if n = 0 * a(n) = 6 * 5^(n-1) , for n > 0 * (Given by
 *            the OEIS entry for A170351)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* n is unsigned; guard against underflow */
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Compute 5^(n-1) */
    seq_t power = seq_pow((seq_t)5, n - 1);

    /* Multiply by 6 */
    return seq_mul(power, (seq_t)6);
}
