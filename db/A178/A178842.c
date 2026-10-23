/*
 * A178842  a(n) = binomial((n-1)^2, n).
 * https://oeis.org/A178842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial((n-1)^2, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Safety: for n == 0 (outside the offset) return 0 */
    if (n == 0) return 0;

    /* Compute (n-1)^2 in idx_t; fits for n up to 10000 and beyond */
    idx_t m = (n - 1) * (n - 1);

    /* binomial(n, k) is provided by the utils library */
    return binomial(m, n);
}
