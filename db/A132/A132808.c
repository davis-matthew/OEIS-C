/*
 * A132808  A001263 * A103451 as infinite lower triangular matrices.
 * https://oeis.org/A132808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangular array obtained from the
 *            Narayana triangle A001263 by replacing its leftmost column with
 *            the Catalan sequence A000108. Thus for each row r ≥ 1 the
 *            entries are: a(row_start + 0) = Catalan(r) // left border
 *            a(row_start + j) = Narayana(r, j+1) // interior entri...
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* find the row r such that n lies in that row */
    idx_t r = 1;
    seq_t tri = 1;                     /* r*(r+1)/2  for r = 1 */
    while (tri < n) {
        ++r;
        tri = (seq_t)r * (r + 1) / 2;
    }

    /* offset of the first element of this row (1‑based) */
    seq_t offset_prev = ((seq_t)(r - 1) * r) / 2;
    idx_t c = (idx_t)(n - offset_prev);     /* column index, 1‑based */

    if (c == 1) {
        /* left border – Catalan number */
        return binom(2 * r, r) / (seq_t)(r + 1);
    } else if ((idx_t)r == c) {
        /* right border */
        return 1;
    } else {
        /* interior entry – Narayana number
           N(r, c) = binom(r, c) * binom(r, c-1) / r
        */
        seq_t val1 = binom((idx_t)r, (idx_t)c);
        seq_t val2 = binom((idx_t)r, (idx_t)c - 1);
        return (val1 * val2) / (seq_t)r;
    }
}
