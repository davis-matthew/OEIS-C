/*
 * A140107  a(n) = binomial(n+3, 3)*7^n.
 * https://oeis.org/A140107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,3) * 7^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute C(n+3,3) = (n+3)*(n+2)*(n+1)/6 */
    seq_t bin = (seq_t)(n + 3) * (n + 2) * (n + 1) / 6;
    /* Compute 7^n */
    seq_t pow7 = pow_uint(7, n);
    /* Multiply */
    return bin * pow7;
}
