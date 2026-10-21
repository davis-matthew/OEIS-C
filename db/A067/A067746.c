/*
 * A067746  Denominator of ((3 * n - 2)/(n^(2 * n - 1) * (2 * n - 1) * 4^(n -
 *          1))).
 * https://oeis.org/A067746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of ((3*n - 2)/(n^(2*n - 1) * (2*n - 1) * 4^(n
 *            - 1)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* powers of two exponents */
    idx_t two_n_minus1 = (n << 1) - 1;   /* 2*n - 1 */
    idx_t two_n_minus2 = (n << 1) - 2;   /* 2*n - 2 */

    /* v2(n) , v2(3*n-2) */
    uint64_t v2n  = __builtin_ctzll(n);            /* n >= 1 */
    uint64_t v2num = __builtin_ctzll(3 * n - 2);    /* (3*n-2) fits in 64-bit */

    /* exponent of 2 in the denominator before cancellation */
    uint64_t denom2_exp = two_n_minus1 * v2n + two_n_minus2;

    /* cancellation amount */
    uint64_t cancel = v2num < denom2_exp ? v2num : denom2_exp;

    /* remaining exponent of 2 */
    idx_t exp_two = two_n_minus2 - cancel;   /* may be zero */

    /* compute n^(2*n-1) */
    seq_t pow_n = big_pow((seq_t)n, two_n_minus1);

    /* compute 2^exp_two */
    seq_t pow_two = big_pow((seq_t)2, exp_two);

    /* assemble result: pow_n * (2*n-1) * pow_two */
    seq_t result = pow_n * (seq_t)two_n_minus1 * pow_two;

    return result;
}
