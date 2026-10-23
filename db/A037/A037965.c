/*
 * A037965  a(n) = n*binomial(2*n-2, n-1).
 * https://oeis.org/A037965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(2*n-2, n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute binomial(2*n-2, n-1) safely */
    idx_t m = 2 * n - 2;  /* m = 2n - 2, safe because n >= 1 */
    idx_t k = n - 1;      /* k = n - 1 */

    seq_t bin = binomial(m, k);
    return (seq_t)n * bin;
}
