/*
 * A062149  Third column of triangle A062138 (generalized a=5 Laguerre).
 * https://oeis.org/A062149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)! * binomial(n+7,7) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* factorial (n+2)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n + 2; ++i) {
        fact *= i;
    }
    /* divide by 2 (factorial of n+2 is always even for n>=0) */
    fact /= 2;

    /* binomial coefficient C(n+7, 7) */
    seq_t bin = binom(n + 7, 7);

    /* final result */
    return fact * bin;
}
