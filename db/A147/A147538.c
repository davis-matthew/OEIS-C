/*
 * A147538  Numbers whose binary representation is the concatenation of n 1's
 *          and 2n-1 digits 0.
 * https://oeis.org/A147538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^(2*n-1) * (2^n - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 2, so n ≥ 1.  We guard against n==0 only to be safe. */
    if (n == 0) return 0;

    /* Compute 2^n.  Use seq_pow because seq_t may be arbitrary‑precision. */
    seq_t pow2n = seq_pow((seq_t)2, n);

    /* 2^(2n-1) = (2^n)^2 >> 1 */
    seq_t pow2n_sq = seq_mul(pow2n, pow2n);
    seq_t pow2_2n_minus1 = pow2n_sq >> 1;

    /* 2^n - 1 */
    seq_t pow2n_minus1 = pow2n - (seq_t)1;

    /* Final result */
    return seq_mul(pow2_2n_minus1, pow2n_minus1);
}
