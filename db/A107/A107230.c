/*
 * A107230  A number triangle of inverse Chebyshev transforms.
 * https://oeis.org/A107230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(row, col) * binomial(row-col,
 *            floor((row-col)/2)) where `row` is the largest integer r such
 *            that r*(r+1)/2 ≤ n, and `col = n - r*(r+1)/2`. This produces the
 *            entries of the triangle T(n,k)=C(n,k)*C(n-k,⌊(n-k)/2⌋) read
 *            row‑wise
 * Range:     u8 n <= 36, u16 n <= 93, u32 n <= 305, u64 n <= 1045,
 *            u128 n <= 3682, big any n
 * Helpers:   binomial, row_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t N, idx_t K) */
#include "../utils/row_from_index.h"

seq_t a(idx_t n)
{
    idx_t row = row_from_index(n);
    idx_t col = n - (idx_t)((unsigned __int128)row * (row + 1) / 2);

    seq_t bin1 = binomial(row, col);          /* C(row, col)          */
    idx_t   rem = row - col;                  /* n-k  remaining steps */
    seq_t bin2 = binomial(rem, rem >> 1);     /* C(rem, floor(rem/2)) */

    return bin1 * bin2;                       /* product of the two binomials */
}
