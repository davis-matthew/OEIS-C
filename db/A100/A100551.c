/*
 * A100551  Coefficient list of ChebyshevU(n, 1-x).
 * https://oeis.org/A100551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   enumerate the coefficients of ChebyshevU(n, 1-x) in row‑major
 *            order. For row r and column c (0 ≤ c ≤ r) the coefficient is
 *            T(r,c) = binomial(r+c+1, r−c) * (−2)^c. The flat index n maps to
 *            the unique pair (r,c) by counting rows of length r+1 (row 0 has
 *            1 element, row 1 has 2, etc.)
 * Range:     i8 n <= 16, i16 n <= 49, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binom_mult, pow2i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_mult.h"
#include "../utils/pow2i.h"

seq_t a(idx_t n)
{
    /* Locate the row r and column c corresponding to the flat index n.  */
    idx_t row = 0;
    idx_t cum = 1;            /* cumulative terms up to the current row (inclusive) */
    while (cum <= n) {
        ++row;
        cum += row + 1;
    }

    idx_t prev_cum = (row * (row + 1)) / 2;
    idx_t col = n - prev_cum;          /* column index within the row, 0‑based */

    seq_t m   = (seq_t)row + (seq_t)col + 1;   /* upper part of binomial */
    seq_t k1  = (seq_t)row - (seq_t)col;       /* lower part of binomial */

    seq_t bin = binom_mult(m, k1);      /* C(m, k1) */
    seq_t pw  = pow2i(col);             /* 2^col */

    seq_t term = bin * pw;
    if (col & 1)                          /* apply the sign (−1)^col */
        term = -term;
    return term;
}
