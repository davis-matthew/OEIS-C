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
 * Validated: standard, pass, 86 terms through n = 3218 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

static seq_t conv_T(idx_t row, idx_t col)
{
    /* Computes T(row,col) for row>=1, col>=1 using the convolution definition. */
    seq_t sum = 0;
    for (idx_t h = 1; h <= col; ++h)
    {
        /* b(h) = 1 + floor(h/2)  → 1 + (h>>1) */
        seq_t bh = (seq_t)(1 + (h >> 1));

        /* c(j-h+1) = floor( (row + (col - h + 1)) / 2 )  → (row + (col - h + 1)) >> 1 */
        seq_t ch = (seq_t)((row + (col - h + 1)) >> 1);

        sum += bh * ch;
    }
    return sum;
}

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
