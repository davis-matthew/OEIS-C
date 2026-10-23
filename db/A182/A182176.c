/*
 * A182176  Number of affine subspaces of GF(2)^n.
 * https://oeis.org/A182176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^{n-k} * [n choose k]_2 , where [n choose
 *            k]_2 = Gaussian binomial coefficient over GF(2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function is O(n).  It uses arbitrary-precision
   arithmetic provided by the seq_t type. */

#include <stdint.h>
#include "../utils/pow2.h"

/* seq_t a(idx_t n) – number of affine subspaces of GF(2)^n */
seq_t a(idx_t n)
{
    seq_t pow2_n   = pow2(n);          /* 2^n                        */
    seq_t gamma    = (seq_t)1;         /* current Gaussian binomial   */
    seq_t sum      = (seq_t)0;         /* running total               */
    seq_t pow2_k1  = (seq_t)2;         /* 2^{k+1}, starts at k=0      */

    for (idx_t k = 0; k <= n; ++k) {
        /* add contribution of dimension k:
           number of affine subspaces dimension k = 2^{n-k} * gamma. */
        sum += gamma * (pow2_n >> k);

        /* stop after the last term (k == n). */
        if (k == n)
            break;

        /* Update gamma to [n choose k+1]_2:
           gamma_{k+1} = gamma_k * (2^{n-k} - 1) / (2^{k+1} - 1). */
        seq_t numerator   = (pow2_n >> k) - (seq_t)1;  /* 2^{n-k} - 1 */
        seq_t denominator = pow2_k1 - (seq_t)1;        /* 2^{k+1} - 1 */
        gamma = (gamma * numerator) / denominator;

        /* prepare 2^{k+2} for the next iteration */
        pow2_k1 <<= 1;   /* shift left by 1 (i.e., multiply by 2) */
    }

    return sum;
}
