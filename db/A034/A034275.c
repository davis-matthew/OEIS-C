/*
 * A034275  a(n) = f(n,n-2) where f is given in A034261.
 * https://oeis.org/A034275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-2,n-1) + (n-1)*binomial(2*n-2,n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely on the utility function binomial(n,k) from the utils library */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Offset is 1; values for n < 1 are undefined. */
    if (n == 0) return 0;

    idx_t two_n_minus2 = 2*n - 2;
    idx_t n_minus1     = n - 1;

    seq_t bin1 = binomial(two_n_minus2, n_minus1);
    seq_t bin2 = binomial(two_n_minus2,   n);

    /* Compute bin1 + (n-1)*bin2 */
    seq_t result = bin1 + (seq_t)n_minus1 * bin2;
    return result;
}
