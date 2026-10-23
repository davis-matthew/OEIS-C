/*
 * A128505  Irregular triangular array a(n,m) for third (k=3) convolution of
 *          Chebyshev's S(n,x) = U(n,x/2) polynomials, read by rows (n >=0, 0
 *          <= m <= floor(n/2)).
 * https://oeis.org/A128505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   a(n) = binom(row‑m+3,3) * binom(row‑m,m) * (‑1)^m where (row,m)
 *            are the row and column indices of the index‑n term in the
 *            flattened triangular array. Row lengths are floor(row/2)+1
 * Range:     i8 n <= 8, i16 n <= 51, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t row = 0;          /* current row index   */
    idx_t cumul = 0;        /* number of terms before this row */

    /* Find the row that contains the nth term (0‑based). */
    for (;;) {
        idx_t row_len = row / 2 + 1;   /* terms in this row   */
        if (cumul + row_len > n)
            break;
        cumul += row_len;
        ++row;
    }

    /* Position of n within the found row. */
    idx_t m = n - cumul;

    /* Compute the binomial factors.  binom returns seq_t. */
    seq_t factor1 = binom(row - m + 3, 3);   /* C(row‑m+3, 3) */
    seq_t factor2 = binom(row - m,     m);   /* C(row‑m,   m) */

    seq_t val = factor1 * factor2;  /* product of the two factors */

    /* Apply the sign (‑1)^m. */
    if (m & 1u)
        val = -val;

    return val;
}
