/*
 * A133510  Number of primitive H-invariant prime ideals in O_q(M_{2,n})
 *          generic quantum matrices.
 * https://oeis.org/A133510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((3^(n+1))-(2^(n+1))+((-1)^(n+1))+2)/4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent n+1 fits in idx_t for all current tests (n up to ~10000) */
    seq_t exp = (seq_t)(n + 1);

    seq_t pow3 = big_pow((seq_t)3, exp);
    seq_t pow2 = big_pow((seq_t)2, exp);

    seq_t num = pow3 - pow2;

    /* add (-1)^(n+1): subtract 1 if n is even, else add 1 */
    if ((n & (idx_t)1) == 0)
        num -= (seq_t)1;
    else
        num += (seq_t)1;

    /* add the constant 2 */
    num += (seq_t)2;

    /* finally divide by 4 */
    return num / (seq_t)4;
}
