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
 * Range:     i8 n <= 16, i16 n <= 49, i32 n <= 178, i64 n <= 647,
 *            i128 n <= 1325, big any n
 * Validated: standard, pass, 71 terms through n = 1303 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline seq_t binom_mult(seq_t n, seq_t k)
{
    /* Compute binomial coefficient C(n,k) multiplicatively.  */
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (seq_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

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
    seq_t pw  = pow2(col);             /* 2^col */

    seq_t term = bin * pw;
    if (col & 1)                          /* apply the sign (−1)^col */
        term = -term;
    return term;
}
