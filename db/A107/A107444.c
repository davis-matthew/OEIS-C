/*
 * A107444  a(n) = C(n^3, n).
 * https://oeis.org/A107444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n^3, n) (binomial coefficient of n^3 choose n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binom_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, but guard against 0 */
    idx_t N = (idx_t)n * n * n;        /* compute n^3; safe for n up to ~10^4 */
    idx_t K = n;
    return binom_mul(N, K);
}
