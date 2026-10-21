/*
 * A227325  a(n) = A000272(n+1) * A000984(n).
 * https://oeis.org/A227325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^(n-1) * C(2*n, n) (valid for n >= 1; a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return 1;

    /* Compute (n+1)^(n-1) */
    seq_t pow = big_pow((seq_t)(n + 1), (seq_t)(n - 1));

    /* Compute binomial coefficient C(2*n, n) */
    seq_t bin = binomial(2 * n, n);

    /* Return the product */
    return pow * bin;
}
