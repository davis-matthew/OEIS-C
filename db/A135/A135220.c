/*
 * A135220  Matrices C_n (read by rows), defined by the following identity:
 *          C_n * P(X,Y) = P(X+Y, X-Y), where P is any homogeneous polynomial
 *          of degree n in two variables, represented as a column whose i-th
 *          element is the coefficient of X^(n-i)Y^i.
 * https://oeis.org/A135220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135220: a(n) is the element read by rows from the matrices C_n,
   where (C_n)_ij = Σ_{k=0..j} (-1)^k * binomial(j,k) * binomial(n-j,i-k).
   Here n is the total index (starting at 1).  The matrix of degree d
   has size (d+1)×(d+1) and contributes (d+1)^2 terms. */

#include <stddef.h>
#include "../utils/binomial.h"  // provides seq_t binomial(idx_t a, idx_t b);

seq_t a(idx_t n) {
    /* n is 1‑based index into the sequence */
    if (n == 0) return 0;              /* undefined for n==0, return 0 */
    idx_t idx = n - 1;                 /* convert to 0‑based position */

    /* find the degree d of the matrix that contains this element */
    idx_t d = 0;                       /* current degree */
    idx_t cum = 0;                     /* terms before this matrix */

    for (;; ++d) {
        idx_t sz = d + 1;              /* matrix size */
        if (cum + sz * sz > idx) break;
        cum += sz * sz;
    }

    idx_t sz   = d + 1;                /* size of matrix C_d   */
    idx_t pos  = idx - cum;            /* position inside C_d   */
    idx_t row  = pos / sz;             /* row index (0‑based)   */
    idx_t col  = pos % sz;             /* column index (0‑based) */

    /* compute the entry using the given formula */
    seq_t sum = 0;
    idx_t n_minus_j = d - col;
    for (idx_t k = 0; k <= col; ++k) {
        if (row < k) continue;                   /* i‑k < 0 → binomial zero */
        seq_t term = binomial(col, k);            /* binom(j,k)   */
        if (k & 1) term = -term;                  /* apply (-1)^k */
        idx_t i_minus_k = row - k;
        if (i_minus_k > n_minus_j) continue;     /* binomial zero */
        term *= binomial(n_minus_j, i_minus_k);  /* * binom(n-j,i-k) */
        sum += term;
    }
    return sum;
}
