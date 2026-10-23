/*
 * A013898  a(n) = 21^(5*n + 1).
 * https://oeis.org/A013898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 5*n + 1 in seq_t */
    seq_t exp = (seq_t)n * 5 + 1;
    /* Return 21 raised to that exponent */
    return big_pow((seq_t)21, exp);
}
