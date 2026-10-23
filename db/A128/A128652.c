/*
 * A128652  Number of square permutations of length n.
 * https://oeis.org/A128652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n+2)*4^(n-3) - 4*(2*n-5)*C(2*n-6,n-3) for n >= 3 *
 *            a(1) = 1, a(2) = 2. The binomial C(2*n-6,n-3) is defined for n
 *            >= 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"        /* provides seq_t pow4(idx_t exp) */
#include "../utils/binomial.h"    /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;               /* a(1) = 1 */
    if (n == 2)
        return 2;               /* a(2) = 2 */

    /* n >= 3 */
    seq_t nseq = (seq_t)n;

    /* term1 = 2*(n+2)*4^(n-3) */
    seq_t term1 = ((seq_t)2 * (nseq + (seq_t)2)) * pow4(n - 3);

    /* term2 = 4*(2*n-5)*C(2*n-6,n-3) */
    seq_t term2 = ((seq_t)4 * ((seq_t)(2 * n - 5)))
                  * binomial(2 * n - 6, n - 3);

    return term1 - term2;
}
