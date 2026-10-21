/*
 * A213783  Rectangular array: (row n) = b**c, where b(h) = 1+[h/2], c(h) =
 *          [(n+h)/2], n>=1, h>=1, [ ] = floor, and ** = convolution.
 * https://oeis.org/A213783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   * Let b(h) = 1 + ⌊h/2⌋ and c(h) = ⌊(n+h)/2⌋. * For each row
 *            index i (n in the OEIS description) and column index j, * the
 *            array entry is the convolution * * T(i,j) = Σ_{h=1}^{j} b(h) *
 *            c(j-h+1) * * The sequence a(n) is obtained by reading the array
 *            along antidiagonals: * first (1,1), then (1,2),(...
 * Range:     u8 n <= 120, u16 n <= 3240, u32 n <= 3240, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   conv_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */
#include "../utils/conv_T.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; no need to handle n==0 */
    /* Find diagonal index d such that d*(d+1)/2 >= n. */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    /* Offset within the diagonal (1‑based) */
    idx_t prev = (d - 1) * d / 2;
    idx_t pos  = n - prev;          /* pos ∈ [1 .. d] */

    /* Coordinates in the array: i = pos, j = d - pos + 1 */
    idx_t i = pos;
    idx_t j = d - pos + 1;

    return conv_T(i, j);
}
