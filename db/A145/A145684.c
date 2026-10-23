/*
 * A145684  Triangle T(n,k) = 2^(1 + floor((n-1)/2)) * A158893(n,k+1).
 * https://oeis.org/A145684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(1+floor((r-1)/2)) * (k == 0 ? 7r-6 : r-k), where r =
 *            smallest integer with r(r+1)/2 ≥ n k = n - r(r-1)/2 - 1 (0‑based
 *            column index)
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Number of terms before this row */
    idx_t prev = r * (r - 1) / 2;
    /* Column index (0‑based) within the row */
    idx_t k = n - prev - 1;

    /* Base value from A158893 */
    seq_t base;
    if (k == 0) {
        base = (seq_t)(7 * (seq_t)r) - 6;
    } else {
        base = (seq_t)(r - k);
    }

    /* Scaling factor: 2^(1+floor((r-1)/2)) */
    idx_t exp = 1 + (r - 1) / 2;
    seq_t scale = pow2(exp);   /* pow2 returns 2^exp as seq_t */

    return scale * base;
}
