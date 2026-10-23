/*
 * A104670  a(n) = binomial(n+2, 2)*binomial(n+7, n).
 * https://oeis.org/A104670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+2,2)*binomial(n+7,n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 381, u128 n <= 1000,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    seq_t term1 = (seq_t)n + 1;          /* n+1 */
    seq_t term2 = (seq_t)n + 2;          /* n+2 */
    seq_t prod;

    /* compute (n+1)(n+2)/2 without overflow */
    if (term1 & 1)                       /* term1 is odd → term2 even */
        prod = term1 * (term2 / 2);
    else
        prod = (term1 / 2) * term2;

    seq_t bin = binom_small((idx_t)n + 7, 7); /* binomial(n+7,7) == binomial(n+7,n) */

    return prod * bin;
}
