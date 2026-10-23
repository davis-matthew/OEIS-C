/*
 * A178300  Triangle T(n, k) = binomial(n+k-1, n) read by rows, 1 <= k <= n.
 * https://oeis.org/A178300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r + k - 1, r) where r is the row index and k is
 *            the column index in the triangle: T(n, k) = binomial(n + k - 1,
 *            n) for 1 ≤ k ≤ n. The function indexes the triangle row‑by‑row
 *            starting at a(1)
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_idx_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset: a(1) is the first term */

    /* Find the row r: minimal r with r*(r+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                               /* row index */
    idx_t prior = (r - 1) * r / 2;              /* terms before row r */
    idx_t k = n - prior;                        /* column index (1‑based) */

    /* Return T(r, k) = binomial(r + k - 1, r) */
    return binomial_idx_t(r + k - 1, r);
}
