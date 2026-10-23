/*
 * A193976  Mirror of the triangle A193975.
 * https://oeis.org/A193976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r be the row index such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
 *            Let t = n – r*(r+1)/2 (position inside the row, 0 ≤ t ≤ r) Let k
 *            = r – t. Then a(n) = w(r,k) = 2·C(k+3,3) + C(k+2,2)·(r−k)
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find row index r */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* offset of the first element in row r */
    idx_t offset = r * (r + 1) / 2;

    /* position inside the row */
    idx_t t = n - offset;

    /* column index in the original triangle (before mirroring) */
    idx_t k = r - t;

    /* compute the term */
    seq_t bin1 = binom(k + 3, 3); /*  C(k+3,3)  */
    seq_t bin2 = binom(k + 2, 2); /*  C(k+2,2)  */

    seq_t term = (seq_t)2 * bin1 + bin2 * (seq_t)t;
    return term;
}
