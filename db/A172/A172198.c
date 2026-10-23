/*
 * A172198  Triangle T(n, k, q) = 1 + abs(c(n,q) - c(k,q))*abs(c(n,q) - c(n-k,
 *          q)), where c(n,q) = Product_{j=1..n} (1 - q^j) and q = 2, read by
 *          rows.
 * https://oeis.org/A172198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = triangle T(n',k) with q = 3 flatten by rows. T(n,k) = 1 +
 *            |c(n) - c(k)| · |c(n) - c(n-k)|, where c(m) = ∏_{j=1..m} (1 -
 *            3^j) (empty product for m=0 = 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   abs_diff_sgn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/abs_diff_sgn.h"

/* Entry a(n) of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Locate row r and column k such that a(n) = T(r,k).  */
    idx_t r = 0;
    idx_t cum = 0;                 /* cumulative number of elements before row r */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;             /* column in row r, 0 ≤ k ≤ r */

    /* Allocate array of magnitudes |c(m)| for m = 0..r.  */
    seq_t *mag = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!mag) return (seq_t)1;     /* unlikely: not expected to run out of memory */

    /* mag[0] = 1 (empty product).  */
    mag[0] = (seq_t)1;

    /* Compute |c(m)| iteratively:
         |c(m)| = |c(m‑1)| * (3^m - 1)   (since each factor 1-3^j is negative). */
    seq_t pow3 = (seq_t)1;               /* holds 3^(m-1) during loop */
    for (idx_t m = 1; m <= r; ++m) {
        pow3 = pow3 * (seq_t)3;          /* 3^m */
        seq_t term = pow3 - (seq_t)1;    /* 3^m - 1 */
        mag[m] = mag[m - 1] * term;
    }

    /* Determine signs of c(r), c(k), c(r-k).  */
    int s_r   = (int)((r & 1) ? -1 : 1);
    int s_k   = (int)((k & 1) ? -1 : 1);
    int s_rk  = (int)(((r - k) & 1) ? -1 : 1);

    /* Compute |c(r) - c(k)| and |c(r) - c(r-k)|.  */
    seq_t diff1 = abs_diff_sgn(s_r, mag[r], s_k, mag[k]);
    seq_t diff2 = abs_diff_sgn(s_r, mag[r], s_rk, mag[r - k]);

    /* Result = 1 + diff1 * diff2.  */
    seq_t result = diff1 * diff2 + (seq_t)1;

    free(mag);
    return result;
}
