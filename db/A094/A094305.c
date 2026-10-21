/*
 * A094305  Triangle read by rows: T(n,k) = ((n+1)(n+2)/2) * binomial(n,k) (0
 *          <= k <= n).
 * https://oeis.org/A094305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ((r+1)(r+2)/2) * binomial(r, k) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r(r+1)/2 (0‑based index
 *            within the row)
 * Range:     u8 n <= 22, u16 n <= 82, u32 n <= 388, u64 n <= 1678,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    /* position k inside the row */
    idx_t k = n - r * (r + 1) / 2;

    /* triangular factor ((r+1)(r+2))/2 as seq_t */
    seq_t fac = ((seq_t)(r + 1) * (r + 2)) / 2;

    /* binomial coefficient C(r, k) */
    seq_t bin = binomial(r, k);

    return fac * bin;
}
