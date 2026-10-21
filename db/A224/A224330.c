/*
 * A224330  Number of idempotent n X n 0..5 matrices of rank n-1.
 * https://oeis.org/A224330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*6^(n-1) - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* a(1) is the first valid term */

    /* Compute 6^(n-1) */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)(n - 1));

    /* Compute 2*pow6 - 1 */
    seq_t tmp = (seq_t)2 * pow6;
    tmp -= (seq_t)1;

    /* Multiply by n */
    seq_t res = tmp * (seq_t)n;
    return res;
}
