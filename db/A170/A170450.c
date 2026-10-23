/*
 * A170450  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 9 * 8^(n-1) for n >= 1 (number of reduced words
 *            of length n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 8^(n-1) using fast exponentiation */
    seq_t pow8 = seq_pow((seq_t)8, n - 1);

    /* Multiply by 9 */
    return (seq_t)9 * pow8;
}
