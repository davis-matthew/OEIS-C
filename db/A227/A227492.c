/*
 * A227492  The Wiener index of the nanostar dendrimer D_n.
 * https://oeis.org/A227492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = - 513 + 2^n*(2016*n + 9468) + 4^n*(4032*n - 8928)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and 4^n */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow4 = pow2 * pow2;          /* 4^n = (2^n)^2 */

    /* Positive contributions */
    seq_t term1 = pow2 * ((seq_t)2016 * (seq_t)n + 9468);   /* 2^n * (2016*n + 9468) */
    seq_t term2 = pow4 * ((seq_t)4032 * (seq_t)n);          /* 4^n * (4032*n) */

    /* Subtract the constant part and the negative term */
    seq_t sub = (seq_t)513 + (pow4 * (seq_t)8928);          /* 513 + 4^n * 8928 */

    /* Final result */
    return term1 + term2 - sub;
}
