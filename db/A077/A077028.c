/*
 * A077028  The rascal triangle, read by rows: T(n,k) (n >= 0, 0 <= k <= n) =
 *          k(n-k) + 1.
 * https://oeis.org/A077028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(r,k) with r = row index where n falls, k = n -
 *            r(r+1)/2, and T(r,k) = k*(r-k) + 1 (this implements the rascal
 *            triangle read by rows)
 * Range:     u8 n <= 541, u16 n <= 8730, u32 n <= 8730, u64 n <= 8730,
 *            u128 n <= 8730, big any n
 * Helpers:   row_index_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_index_from_n.h"

seq_t a(idx_t n) {
    /* Determine row r and column k for the n‑th element (0‑based). */
    idx_t r = row_index_from_n(n);

    /* Starting index of row r. */
    seq_t row_start_seq = ((seq_t)r * (r + 1)) / 2;
    idx_t row_start = (idx_t)row_start_seq; /* fits in idx_t by construction */

    idx_t k = n - row_start;                 /* column within the row */

    /* Compute the triangle entry: T(r,k) = k*(r-k) + 1. */
    seq_t term = ((seq_t)k * (seq_t)(r - k)) + 1;
    return term;
}
