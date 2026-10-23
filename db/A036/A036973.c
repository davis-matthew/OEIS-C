/*
 * A036973  (7*n^3+4*n^2+4*n)*binomial(2*n,n)/30.
 * https://oeis.org/A036973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*n^3+4*n^2+4*n)*binomial(2*n,n)/30
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for the arithmetic */
    seq_t nn = (seq_t)n;

    /* coefficient (7*n^3 + 4*n^2 + 4*n) */
    seq_t coeff = (seq_t)7 * nn * nn * nn
                + (seq_t)4 * nn * nn
                + (seq_t)4 * nn;

    /* binomial(2*n , n) */
    idx_t two_n = n + n;
    seq_t bin = binomial(two_n, n);

    /* product and division by 30 (exact division) */
    seq_t prod = coeff * bin;
    seq_t result = prod / 30;

    return result;
}
