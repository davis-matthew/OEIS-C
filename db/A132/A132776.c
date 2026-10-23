/*
 * A132776  A128064 (unsigned) * A007318.
 * https://oeis.org/A132776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1)*C(r,c) + r*C(r-1,c) where r = ⌊(√(8n+1)-1)/2⌋ and c
 *            = n - r(r+1)/2 (C(i,j) is Pascal's triangle, computed by
 *            binom(i,j). The values come from the product (lower bidiagonal)
 *            * (Pascal) triangular matrices.)
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t tri = r * (r + 1) / 2;   /* triangular number of row r */
    idx_t c   = n - tri;           /* column within the row */

    seq_t bin_rc = binom(r, c);

    seq_t bin_r_minus1_c = 0;
    if (r > 0 && c <= r - 1)
        bin_r_minus1_c = binom(r - 1, c);

    seq_t term1 = bin_rc * (seq_t)(r + 1);
    seq_t term2 = bin_r_minus1_c * (seq_t)r;

    return term1 + term2;
}
