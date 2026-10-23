/*
 * A131230  Triangle read by rows: 2*A130296 - A128174.
 * https://oeis.org/A131230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   tri_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A131230
 *
 * The triangle is defined by
 *   T(r,1) = 4*(r/2) + (r & 1)                     // first column: numbers ≡0 or 1 (mod 4)
 *   T(r,j) = 1  if (r % 2 == j % 2) else 2          // for j ≥ 2
 *
 * The sequence a(n) is the triangle read by rows (flattened).
 * For an index n (1‑based) we find the unique row r with
 *   r(r‑1)/2 < n ≤ r(r+1)/2,
 * then j = n – r(r‑1)/2.  The value is T(r,j).
 */

#include <stdint.h>
#include "../utils/tri_uint64.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r‑1)/2 < n ≤ r(r+1)/2 */
    idx_t low = 1, high = 1;
    while (tri_uint64(high) < n) {
        high <<= 1;               // double until upper bound reached
    }

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (tri_uint64(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     // row index

    /* Compute previous triangular number (r‑1)*r/2 */
    idx_t prev_tri = (idx_t)(tri_uint64(r - 1));

    idx_t j = n - prev_tri;            // column index (1‑based)

    if (j == 1) {
        /* first column: 4*(r/2) + (r & 1) */
        seq_t val = (seq_t)(4 * (r / 2) + (r & 1));
        return val;
    } else {
        /* j ≥ 2 */
        return (seq_t)(((r & 1) == (j & 1)) ? 1 : 2);
    }
}
