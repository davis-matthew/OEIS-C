/*
 * A126020  Number of convex permutominoes of size n.
 * https://oeis.org/A126020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = 2*(n+3)*4^(n-2) - (n*binomial(2*n,n))/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"     /* provides seq_t pow4(idx_t exp)          */
#include "../utils/binomial.h"/* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 separately, because 4^(n-2) would
       require a negative exponent. */
    if (n == 1)
        return 1;

    /* First term: 2*(n+3)*4^(n-2) */
    seq_t term1 = (seq_t)2 * (seq_t)(n + 3) * pow4(n - 2);

    /* Binomial coefficient: binomial(2*n, n) */
    seq_t bin = binomial(2 * n, n);

    /* Second term: (n * bin) / 2  (the division is exact) */
    seq_t term2 = ((seq_t)n * bin) / 2;

    return term1 - term2;
}
