/*
 * A160791  a(n) = binomial(N, n - N) where N = 1 + floor(n/2).
 * https://oeis.org/A160791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(1 + floor(n/2), n - (1 + floor(n/2)))
 * Range:     u8 n <= 31, u16 n <= 723, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special case: a(0) = 0 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) = 0 by definition */
    idx_t N = 1 + (n >> 1);           /* N = 1 + floor(n/2) */
    idx_t k = n - N;                  /* k = n - N = floor((n-1)/2) */
    return binomial(N, k);            /* compute binomial(N, k) */
}
