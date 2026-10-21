/*
 * A085284  a(n) = C(n+3,3)*n^3/4.
 * https://oeis.org/A085284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+3,3)*n^3/4
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute binomial part C(n+3,3) */
    seq_t res = binom(n + 3, 3);

    /* Multiply by n^3, using seq_t arithmetic */
    seq_t nn = (seq_t)n;
    res *= nn * nn * nn;

    /* Divide by 4 – the result is always divisible by 4 */
    res /= 4;

    return res;
}
