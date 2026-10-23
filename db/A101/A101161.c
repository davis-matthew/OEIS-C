/*
 * A101161  A number triangle associated with the Chebyshev polynomials of the
 *          first kind.
 * https://oeis.org/A101161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = S(r, k), where S(n, k) is the OEIS A101161 triangle read
 *            in row‑major order. Here r = ⌊(√(8n+1)-1)/2⌋ and k = n -
 *            r(r+1)/2. For each fixed k the sequence S(k+m, k) (m ≥ 0)
 *            satisfies S(k, k) = 1, S(k+1, k) = k+2, S(k+2, k) = (k+2)·S(k+1,
 *            k) – S(k, k) – 1, S(k+m, k) = (k+2)·S(k+m-1, k) – S(k...
 * Range:     u8 n <= 28, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"   /* provides int_sqrt(idx_t) */

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t t = int_sqrt(8 * n + 1);
    idx_t r = (t - 1) / 2;
    /* Correct possible off‑by‑one errors from integer sqrt */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column index k */
    idx_t k = n - r * (r + 1) / 2;

    /* Direct return for the diagonal */
    if (r == k) return (seq_t)1;

    /* d = number of steps away from the diagonal */
    idx_t d = r - k;
    seq_t k_plus2 = (seq_t)(k + 2);

    /* Handle first two steps explicitly */
    if (d == 0) return (seq_t)1;   /* S(k, k)   */
    if (d == 1) return k_plus2;    /* S(k+1, k) */

    /* Recurrence for m ≥ 2 */
    seq_t prev2 = (seq_t)1;   /* S(k, k)   */
    seq_t prev1 = k_plus2;    /* S(k+1, k) */
    seq_t cur;

    for (idx_t m = 2; m <= d; ++m) {
        if (m == 2)
            cur = k_plus2 * prev1 - prev2 - (seq_t)1;  /* special case m = 2 */
        else
            cur = k_plus2 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* S(r, k) */
}
