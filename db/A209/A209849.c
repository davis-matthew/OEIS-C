/*
 * A209849  Triangle read by rows: coefficients of polynomials in Sum_{k =
 *          0..t} k^n * binomial(t,k).
 * https://oeis.org/A209849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the coefficient of t^pos in polynomial R(r,t) with r the
 *            row index from the triangular numbering (1 ≤ r) and pos = n – r
 *            (r–1)/2. The polynomials satisfy R(0,t)=1 and
 *            R(n+1,t)=t*(2*R(n,t)-R(n,t-1))
 * Range:     i8 n <= 23, i16 n <= 56, i32 n <= 121, i64 n <= 301,
 *            i128 n <= 744, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* find the row r and position pos within that row */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev = r * (r - 1) / 2;
    idx_t pos  = n - prev;   /* 1‑based position in the row */

    /* allocate small buffers (row number < 200 for n <= 10000) */
    seq_t cur[200];
    seq_t next[200];
    seq_t shift[200];

    /* initial row (n = 1) : R(1,t) = t */
    cur[0] = 0;
    cur[1] = 1;          /* degree = 1 */

    /* compute rows up to r */
    for (size_t row = 1; row < (size_t)r; ++row) {
        size_t degree = row;          /* current row degree */
        size_t shift_len = degree + 1; /* indices 0..degree */

        /* zero shift buffer */
        for (size_t k = 0; k <= degree; ++k) shift[k] = 0;

        /* build shift coefficients: Coefficients of R(n,t-1) */
        for (size_t i = 1; i <= degree; ++i) {
            seq_t bin = 1;                 /* binom(i,0) */
            seq_t coeff_c = cur[i];
            for (size_t k = 0; k <= i; ++k) {
                int sign = ((i - k) & 1) ? -1 : 1;
                seq_t term = coeff_c * bin;
                if (sign < 0) shift[k] -= term; else shift[k] += term;
                if (k < i) {
                    seq_t num = (seq_t)(i - k);
                    seq_t den = (seq_t)(k + 1);
                    bin = bin * num / den;
                }
            }
        }

        /* compute next row (degree+1) */
        next[0] = 0;
        next[1] = - shift[0];
        for (size_t k = 1; k <= degree; ++k) {
            seq_t s = cur[k] * 2 - shift[k];
            next[k + 1] = s;
        }

        /* copy next to cur for next iteration */
        for (size_t i = 0; i <= degree + 1; ++i) cur[i] = next[i];
    }

    /* cur[pos] is the desired coefficient */
    return cur[(size_t)pos];
}
