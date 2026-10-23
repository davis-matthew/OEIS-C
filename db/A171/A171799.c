/*
 * A171799  O.g.f.: Sum_{n>=0} 2^(n^2)*x^n/(1 - 2^n*x)^n.
 * https://oeis.org/A171799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (2^n + 1)^(n-1) for n > 0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 */

    /* compute 2^n */
    seq_t pow2n = big_pow((seq_t)2, (seq_t)n);

    /* compute (2^n + 1)^(n-1) */
    seq_t base   = pow2n + (seq_t)1;
    seq_t pow_term = pow_seq(base, (int)(n - 1));

    /* a(n) = 2^n * (2^n + 1)^(n-1) */
    return pow2n * pow_term;
}
