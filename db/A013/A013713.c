/*
 * A013713  a(n) = 8^(2n+1).
 * https://oeis.org/A013713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(2n+1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 2n+1 using seq_t arithmetic */
    seq_t exp = (seq_t)n * 2 + 1;
    /* Compute 8^(2n+1) */
    return big_pow((seq_t)8, exp);
}
