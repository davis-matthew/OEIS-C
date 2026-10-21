/*
 * A200613  Number of quasi-abelian ideals in the affine Lie algebra
 *          sl_n^{hat}.
 * https://oeis.org/A200613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+4)/4)*binomial(2*n,n)-3*pow2(2*n-3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1.  For n < 1 the function is not used. */
    if (n < 1)
        return 0;                /* undefined range, harmless */

    if (n == 1)
        return 1;                /* the only special case where 2*n-3 < 0 */

    /* Compute the binomial coefficient C(2n,n). */
    seq_t binom = binomial(2*n, n);            /* C(2n, n) */

    /* First term: ((n+4)/4) * C(2n,n).  The division by 4 is exact
       for all n (the product is always divisible by 4). */
    seq_t term1 = ((seq_t)(n + 4) * binom) / 4;

    /* Second term: 3 * 2^(2n-3).  2n-3 is positive for n >= 2. */
    seq_t term2 = 3 * pow2(2*n - 3);

    return term1 - term2;
}
