/*
 * A200073  Coefficients of a generalized Jaco-Lucas polynomial (odd indices)
 *          read by rows.
 * https://oeis.org/A200073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattening of the triangle T(n,k) defined by * T(n,k) =
 *            Σ_{j=0..n} (2n+1)*C(2n+1-j, j)*C(j,k)/(2n+1-j) * where n is the
 *            row index and k the column index (0 ≤ k ≤ n). The * sequence
 *            a(n) is obtained by reading the triangle row by row
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k such that n = r*(r+1)/2 + k, with
     * 0 ≤ k ≤ r.  Use binary search on r; the values for n ≤ 10 000
     * are far below 2^32, so 64‑bit arithmetic is safe. */
    idx_t low = 0;
    idx_t high = n;
    idx_t r = 0;

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t k = n - r * (r + 1) / 2;

    /* Evaluate T(r,k) according to the given formula. */
    seq_t total = 0;
    seq_t two_r_plus1 = (seq_t)(2 * r + 1);

    for (idx_t j = 0; j <= r; ++j) {
        seq_t b1 = binomial(2 * r + 1 - j, j);
        seq_t b2 = binomial(j, k);
        if (!b2) continue;                  /* C(j,k) = 0 if k > j */
        seq_t product = b1 * b2;
        product *= two_r_plus1;
        seq_t denom = (seq_t)(2 * r + 1 - j);
        total += product / denom;
    }

    return total;
}
