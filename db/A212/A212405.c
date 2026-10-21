/*
 * A212405  Number of binary arrays of length 2*n+3 with no more than n ones
 *          in any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 13 for n > 1: a(n) = 2^(2*n+2) - ((3*n+1)/n) * C(2*n-2,
 *            n-1) where C(x, y) is the binomial coefficient. This is the
 *            sequence A212405. The implementation uses the supplied utility
 *            functions binomial() and pow2(). For n == 1 the value is
 *            returned directly
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the utility headers for binomial and pow2 */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* external linkage function as required */
seq_t a(idx_t n)
{
    if (n == 1)            /* a(1) = 13 */
        return (seq_t)13;

    /* compute the binomial coefficient C(2*n-2, n-1) */
    idx_t N = 2 * n - 2;
    idx_t K = n - 1;
    seq_t bin = binomial(N, K);

    /* compute (3*n+1)/n * binomial */
    seq_t factor = ((seq_t)(3 * n + 1)) * bin / (seq_t)n;

    /* 2^(2*n+2) */
    idx_t exp = 2 * n + 2;
    seq_t two_pow = pow2(exp);

    /* result */
    return two_pow - factor;
}
