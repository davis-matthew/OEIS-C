/*
 * A119364  Central coefficients of number triangle A119335.
 * https://oeis.org/A119364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} C(n+1,3k) * C(n-1,3k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* special case: a(0) = 0 */
    if (n == 0U)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    /* maximum k such that 3k <= n-1 */
    idx_t maxk = (n - 1U) / 3U;

    for (idx_t k = 0U; k <= maxk; ++k) {
        idx_t t = 3U * k;
        seq_t c1 = binom(n + 1U, t); /* C(n+1, 3k)   */
        seq_t c2 = binom(n - 1U, t); /* C(n-1, 3k)   */
        sum += c1 * c2;
    }
    return sum;
}
