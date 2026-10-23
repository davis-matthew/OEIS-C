/*
 * A171002  Binomial(n-k,k) * binomial(n-k-1,k+1) where k = ceiling(n/4).
 * https://oeis.org/A171002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-k, k) * binomial(n-k-1, k+1) where k =
 *            ceil(n/4)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* k = ceil(n/4) = (n+3)/4 for all n >= 1 */
    idx_t k = (n + 3) / 4;

    /* If n < k, the first binomial is undefined (n-k < 0) */
    if (n < k)
        return 0;

    idx_t nk = n - k;               /* n-k >= 0 */

    /* For the second binomial we need n-k-1 >= k+1  ⇔  n-k >= k+2 */
    if (nk < k + 2)
        return 0;

    seq_t x = binomial(nk, k);
    seq_t y = binomial(nk - 1, k + 1);
    return x * y;
}
