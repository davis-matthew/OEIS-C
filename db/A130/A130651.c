/*
 * A130651  1/16 the number of permutations of 0..n having exactly 3 maxima.
 * https://oeis.org/A130651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-8) * ( 3^(n+1) - 2^(n+2)*n + 2*n^2 - 3 )
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 3^(n+1) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n + 1));

    /* 2^(n+2) */
    seq_t pow2_nplus2 = big_pow((seq_t)2, (seq_t)(n + 2));

    /* term = pow3 - pow2_nplus2 * n + 2*n*n - 3 */
    seq_t term = pow3;
    term -= pow2_nplus2 * (seq_t)n;
    term += 2 * ((seq_t)n * (seq_t)n);
    term -= (seq_t)3;

    /* multiply or divide by 2^(n-8) */
    if (n >= 8)
    {
        return term << (n - 8);
    }
    else
    {
        return term >> (8 - n);
    }
}
