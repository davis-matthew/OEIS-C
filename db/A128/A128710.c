/*
 * A128710  Triangle read by rows: T(n,k) = (k+2)*binomial(n,k) (0 <= k <= n).
 * https://oeis.org/A128710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+2) * C(r,k) where * r = max { m | m*(m+1)/2 ≤ n } * k
 *            = n - r*(r+1)/2 * C(r,k) is the binomial coefficient. * This
 *            concatenates the rows of the triangle T(n,k)= (k+2)*C(n,k)
 * Range:     u8 n <= 38, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the 0‑based value a(n) for the sequence A128710 */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute binomial(r,k) multiplicatively.  Use the symmetry
     * C(r,k) = C(r,r-k) to keep the loop short. */
    idx_t k_small = k;
    if (k_small > r - k_small) {
        k_small = r - k_small;
    }

    seq_t bin = 1;
    for (idx_t i = 1; i <= k_small; ++i) {
        /* bin = bin * (r - k_small + i) / i  (exact division) */
        seq_t numerator   = (seq_t)(r - k_small + i);
        seq_t denominator = (seq_t)i;

        bin = bin * numerator / denominator;
    }

    /* Return the triangle entry. */
    return (seq_t)(k + 2) * bin;
}
