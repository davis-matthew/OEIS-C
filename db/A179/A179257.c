/*
 * A179257  Number of permutations of length n which avoid the patterns 321
 *          and 1324.
 * https://oeis.org/A179257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + binomial(n,2) + binomial(n+2,5) (OEIS A179257)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    seq_t result = (seq_t)1;

    /* binomial(n,2) contributes 0 for n<2, which is handled by binom() */
    result += binom(n, 2);

    /* binomial(n+2,5) contributes 0 for n+2<5, handled by binom() */
    result += binom(n + 2, 5);

    return result;
}
