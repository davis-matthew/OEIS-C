/*
 * A104731  Triangle T(n,k) = sum_{j=k..n} (j+1)*binomial(k,j-k), read by
 *          rows, 0<=k<=n.
 * https://oeis.org/A104731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{t=0}^{r-k} (k+t+1)*C(k,t) where * r =
 *            floor((√(8*n+1)-1)/2) (the row of n in the triangle) * k = n -
 *            r*(r+1)/2 (the column in that row)
 * Range:     u8 n <= 49, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   compute_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_row.h"

seq_t a(idx_t n) {
    /* Find row r and column k of the flattened triangle. */
    idx_t r = compute_row(n);
    idx_t shift = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - shift;

    /* Compute T(r,k) = Σ_{t=0}^{r-k} (k+t+1)*C(k,t). */
    idx_t max_t = r - k;
    seq_t sum = 0;
    seq_t bin = 1;          /* bin = C(k,0) */

    for (idx_t t = 0; t <= max_t; ++t) {
        sum += ((seq_t)(k + t + 1)) * bin;
        if (t < max_t) {
            if (t < k) {
                bin = (bin * (k - t)) / (t + 1);   /* bin = C(k,t+1) */
            } else {
                bin = 0;                           /* C(k,t) = 0 for t > k */
            }
        }
    }

    return sum;
}
