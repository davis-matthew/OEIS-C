/*
 * A185734  Third accumulation array of the polygonal number array (A086270),
 *          by antidiagonals.
 * https://oeis.org/A185734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n',k') where * T(n,k) = binom(k+3,4) * binom(n+2,3) * *
 *            (k*n - n + 3*k + 17) / 20 * enumerated by antidiagonals of the
 *            (n,k)–plane
 * Range:     u8 n <= 10, u16 n <= 82, u32 n <= 682, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find diagonal d such that n lies in
       the antidiagonal of sum (n+k) = d+1   */
    idx_t d   = 1;          /* current diagonal (1‑based)   */
    idx_t cum = 1;          /* cumulative number of terms
                               up to and including this diagonal */
    while (cum < n) {
        ++d;
        cum += d;
    }

    /* terms preceding this diagonal */
    idx_t prev = cum - d;
    /* position of n inside the diagonal (0‑based) */
    idx_t pos  = n - prev - 1;

    /* coordinates in the (n,k) grid */
    idx_t row = pos + 1;          /* n‑coordinate (row)   */
    idx_t col = d - pos;          /* k‑coordinate (col)   */

    /* compute T(row, col) */
    seq_t c1 = binom(col + 3, 4);      /* C(col+3,4) */
    seq_t c2 = binom(row + 2, 3);      /* C(row+2,3) */
    seq_t m  = (seq_t)col * (seq_t)row
               - (seq_t)row
               + 3 * (seq_t)col + 17;   /* (k*n - n + 3*k + 17) */

    return c1 * c2 * m / 20;
}
