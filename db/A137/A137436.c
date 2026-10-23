/*
 * A137436  Triangular sequence based on the coefficients of the Blaschke
 *          product like tan(3u) polynomial function: p(x,t)=Exp[x*t]*(-t)*(3
 *          - t^2)/(-1 + 3*t^2).
 * https://oeis.org/A137436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = r_{N-m} * (N! / m!) where N = row index – 1, m = column
 *            index in the row, and r_k = 3 if k = 1, 8·3^{(k-3)/2} if k ≥ 3
 *            is odd, 0 if k is even or 0. The row index r is the unique
 *            integer such that the cumulative number of terms up to row r is
 *            ≥ n, with a row of length 1 for r = 1 and length...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;            /* out of range, safe zero */

    /* Find the row r that contains the n‑th term.  The cumulative
       number of terms up to row r is: 1 + (r-1)(r-2)/2 + 1 for r ≥ 2.
       We march until the cumulative count reaches n. */
    idx_t r = 1;
    idx_t cum = 1;                           /* terms up to current row */
    while (cum < n) {
        ++r;
        cum += (r == 1 ? 1 : r - 1);          /* add length of next row */
    }

    /* Length of this row and its starting index. */
    idx_t row_len = (r == 1 ? 1 : r - 1);
    idx_t start = cum - row_len + 1;          /* first index in this row */

    /* Position inside the row (0‑based). */
    idx_t m = n - start;                      /* 0 ≤ m < row_len */
    if (m >= row_len) return (seq_t)0;       /* safety guard */

    /* Polynomial index N (row index minus one). */
    idx_t N = r - 1;

    /* The k used in the coefficient r_k.  k = N - m. */
    if (m > N) return (seq_t)0;               /* safety guard */
    idx_t k = N - m;

    /* Compute r_k.  It is zero for even k or k == 0. */
    seq_t coeff_R;
    if (k == 0) return (seq_t)0;
    if ((k & 1) == 0) return (seq_t)0;        /* even k → 0 */

    if (k == 1) {
        coeff_R = (seq_t)3;
    } else {
        /* k is odd and ≥ 3: r_k = 8 * 3^{(k-3)/2}. */
        idx_t exp = (k - 3) / 2;
        seq_t pow3 = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i) {
            pow3 *= (seq_t)3;
        }
        coeff_R = (seq_t)8 * pow3;
    }

    /* Compute N! / m! as product from m+1 to N. */
    seq_t factor = (seq_t)1;
    if (m < N) {
        for (idx_t t = m + 1; t <= N; ++t) {
            factor *= (seq_t)t;
        }
    }

    return coeff_R * factor;
}
