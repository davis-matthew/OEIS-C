/*
 * A133224  Let P(A) be the power set of an n-element set A and let B be the
 *          Cartesian product of P(A) with itself. Remove (y,x) from B when
 *          (x,y) is in B and x <> y and let R35 denote the reduced set B.
 *          Then a(n) = the sum of the sizes of the union of x and y for every
 *          (x,y) in R35.
 * https://oeis.org/A133224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (2^(n-2) + 3 * 2^(2*n-3)) (n ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"  /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Handle small cases where the exponent would be negative */
    if (n == 0) return 0;
    if (n == 1) return 2;

    /* Compute the powers of two using the utility function */
    seq_t pow1 = pow_uint((uint64_t)2, n - 2);           /* 2^(n-2)      */
    seq_t pow2 = pow_uint((uint64_t)2, 2 * n - 3);       /* 2^(2n-3)     */
    pow2 = pow2 * 3;                                    /* 3 * 2^(2n-3) */

    /* Sum the two parts and multiply by n */
    seq_t sum = pow1 + pow2;
    seq_t result = (seq_t)n * sum;

    return result;
}
