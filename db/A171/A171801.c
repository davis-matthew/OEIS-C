/*
 * A171801  O.g.f.: Sum_{n>=0} (n+1)*2^(n^2)*x^n/(1 - 2^n*x)^n.
 * https://oeis.org/A171801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * ((n+1)*2^n + 2) * (2^n + 1)^(n-2) for n>0, with
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* 2^n */
    seq_t pow2 = pow_uint(2, n);
    /* (2^n + 1)^(n-2) */
    seq_t term2 = big_pow(pow2 + (seq_t)1, (seq_t)(n - 2));
    /* ((n+1)*2^n + 2) */
    seq_t inner = ((seq_t)(n + 1)) * pow2 + (seq_t)2;
    /* Final product */
    seq_t result = pow2 * inner;
    result = result * term2;
    return result;
}
