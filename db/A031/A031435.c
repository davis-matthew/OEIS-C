/*
 * A031435  Reversal point for powers of consecutive natural numbers.
 * https://oeis.org/A031435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( log(1+n) / log(1+1/n) )
 * Range:     u8 n <= 61, u16 n <= 7359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
seq_t a(idx_t n)
{
    /* Offset: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Use double precision to evaluate the logarithm ratio. */
    double dn = (double)n;
    double val = log1p(dn) / log1p(1.0 / dn);

    /* Small epsilon to avoid rounding down when val is very close to an integer. */
    const double eps = 1e-12;
    idx_t k = (idx_t)(floor(val + eps));

    return (seq_t)k;
}
