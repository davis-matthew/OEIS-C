/*
 * A119968  Binomial transform of Fredholm-Rueppel sequence.
 * https://oeis.org/A119968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k≥0, 2^k‑1 ≤ n} binom(n, 2^k‑1)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = 1;                       /* m = 2^k */
    while (m - 1 <= n)
    {
        idx_t k = m - 1;               /* k = 2^k - 1 */
        sum += binom(n, k);
        m <<= 1;                       /* next power of two */
    }
    return sum;
}
