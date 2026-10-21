/*
 * A082652  Triangle read by rows: T(n,k) is the number of squares that can be
 *          found in a k X n rectangular grid of little squares, for 1 <= k <=
 *          n.
 * https://oeis.org/A082652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) where the triangle is read by rows. * r =
 *            ceil((sqrt(8·n+1)-1)/2) (row index) * k = n - r·(r-1)/2 (column
 *            index within the row) * T(r,k) = Σ_{i=0}^{k} (r-i)(k-i) *
 *            (equivalent to T(r,k) = (k+3k·r+3k²·r-k³)/6) * 1‑based indexing;
 *            a(1) = 1, a(2) = 2, a(3) = 5, …
 * Range:     u8 n <= 44, u16 n <= 1710, u32 n <= 9587, u64 n <= 9587,
 *            u128 n <= 9587, big any n
 * Helpers:   row_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of_idx.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row (r) and position (k) in that row. */
    idx_t r       = row_of_idx(n);                         /* 1‑based row */
    idx_t prev    = (unsigned __int128)(r - 1) * r / 2;     /* elements before row r */
    idx_t k       = n - prev;                               /* 1‑based column in the row */
    idx_t i;

    /* Compute T(r,k) = Σ_{i=0}^{k} (r-i)(k-i).  Use seq_t for all arithmetic. */
    seq_t sum = 0;
    for (i = 0; i <= k; ++i) {
        sum += (seq_t)(r - i) * (seq_t)(k - i);
    }
    return sum;
}
