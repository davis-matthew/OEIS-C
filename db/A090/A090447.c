/*
 * A090447  Triangle of partial products of binomials.
 * https://oeis.org/A090447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ∏_{k=0}^{m} binomial(r, k), where (r, m) are the row and
 *            column indices obtained by writing n as the flattened index of
 *            the triangle
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 42, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Determine row r and column m from the linear index n */
    idx_t row   = 0;   /* current row number (0‑based)   */
    idx_t start = 0;   /* starting linear index of this row */
    idx_t len   = 1;   /* number of elements in current row = row + 1 */

    while (start + len <= n) {
        start += len;
        ++len;
        ++row;
    }

    idx_t col = n - start;   /* column index within the found row */

    /* Compute the product of binomials up to this column */
    seq_t prod = 1;
    for (idx_t k = 0; k <= col; ++k) {
        prod = prod * binomial(row, k);
    }
    return prod;
}
