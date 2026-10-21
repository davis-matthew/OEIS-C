/*
 * A228890  Triangular array read by rows. T(n,k) is the number of 2-colored
 *          labeled graphs on n nodes with exactly k edges; n >= 0, 0 <= k <=
 *          A002620(n).
 * https://oeis.org/A228890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row,col) where row and col are determined by the
 *            triangular array of 2‑colored graphs. T(row,col) =
 *            sum_{b=0}^{row} C(row,b) * C(b*(row-b),col) (OEIS A228890)
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row (row index r) such that the accumulated number of
       terms up to row r-1 is <= n < accumulated terms up to r.  */
    idx_t r = 0;
    idx_t acc = 0;                   /* number of terms before current row */

    while (1) {
        idx_t len = (r * r) / 4 + 1; /* floor(r^2/4)+1 terms in this row */
        if (acc + len > n)
            break;
        acc += len;
        ++r;
    }

    /* Position within the row described by r */
    idx_t col = n - acc;

    /* Compute T(r,col) */
    seq_t total = 0;

    for (idx_t b = 0; b <= r; ++b) {
        idx_t cross = b * (r - b);     /* number of possible edges for this split */
        if (cross < col)          /* need at least 'col' edges */
            continue;
        seq_t tb = binomial(r, b);      /* choose which vertices are black */
        seq_t cb = binomial(cross, col);/* choose which edges are present */
        total += tb * cb;
    }
    return total;
}
