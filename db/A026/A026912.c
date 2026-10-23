/*
 * A026912  a(n) = A026907(2*n-1,n-1).
 * https://oeis.org/A026912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-1, n-1) + 3*binomial(2*n+3, n+1) - 18
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Assume n ≥ 1 */
    seq_t term1 = binomial(2*n - 1, n - 1);
    seq_t term2 = binomial(2*n + 3, n + 1);
    term2 = term2 * (seq_t)3;
    return term1 + term2 - (seq_t)18;
}
