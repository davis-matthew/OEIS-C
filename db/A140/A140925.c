/*
 * A140925  a(n) = binomial(m+n-1,n)^2 - binomial(m+n,n+1)*binomial(m+n-2,n-1)
 *          with m=12.
 * https://oeis.org/A140925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(12+n-1, n)^2 - binomial(12+n,
 *            n+1)*binomial(12+n-2, n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 41, u128 n <= 425,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Handle the only special case explicitly */
    if (n == 0) {
        return 1;
    }

    /* Compute the three binomial coefficients */
    seq_t bin1 = binom(n + 11, n);      /* C(12 + n - 1, n)   */
    seq_t bin2 = binom(n + 12, n + 1);  /* C(12 + n, n + 1)   */
    seq_t bin3 = binom(n + 10, n - 1);  /* C(12 + n - 2, n - 1) */

    /* Square the first coefficient */
    seq_t sq1 = bin1 * bin1;

    /* Multiply the other two coefficients */
    seq_t prod = bin2 * bin3;

    /* Return the difference */
    return sq1 - prod;
}
