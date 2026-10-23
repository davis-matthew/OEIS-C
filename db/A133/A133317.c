/*
 * A133317  Dimensions of certain Lie algebra (see reference for precise
 *          definition).
 * https://oeis.org/A133317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+9,9) + 25*C(n+8,9) + 100*C(n+7,9) + 100*C(n+6,9) +
 *            25*C(n+5,9) + C(n+4,9) where C(a,b) denotes the binomial
 *            coefficient
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 23, u64 n <= 307,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* term for shift = 0 */
    sum += binom(n + 9, 9);

    /* shift = 1 */
    sum += 25 * binom(n + 8, 9);

    /* shift = 2 */
    sum += 100 * binom(n + 7, 9);

    /* shift = 3 */
    sum += 100 * binom(n + 6, 9);

    /* shift = 4 */
    sum += 25 * binom(n + 5, 9);

    /* shift = 5 */
    sum += binom(n + 4, 9);   /* binom will return 0 if argument < 9 */

    return sum;
}
