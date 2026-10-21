/*
 * A204170  Array read by rows: row n lists the coefficients of the
 *          characteristic polynomial of the n-th principal submatrix of
 *          (i*j), as in A003991.
 * https://oeis.org/A204170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(n) = coefficient of x^(k) in the characteristic * polynomial
 *            of the n‑th principal submatrix of (i*j), read row‑by‑row. * For
 *            row r (1‑based), the coefficients are: * 0 (for powers 0 … r‑2),
 *            * (-1)^(r+1) * Σ_{i=1}^r i^2 (for power r‑1), * (-1)^r (for
 *            power r)
 * Range:     i8 n <= 33, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that S(r‑1) < n <= S(r) where
       S(r) = r(r+3)/2 is the cumulative number of terms up to row r. */
    idx_t r = 0;
    while ((r * (r + 3)) / 2 < n) {
        r++;
    }

    /* Number of terms before this row */
    idx_t prev = ((r - 1) * (r + 2)) / 2;

    /* 0‑based position inside the row */
    idx_t pos = n - prev - 1;

    /* Every row has length r+1.  The first r‑1 positions are zeros. */
    if (pos < r - 1) {
        return 0;
    }

    /* Position r‑1: coefficient of x^(r‑1) = (-1)^(r+1) * S(r) */
    if (pos == r - 1) {
        seq_t s = (seq_t)r * (seq_t)(r + 1) * (seq_t)(2 * r + 1) / 6; /* sum of squares */
        if ((r & 1) == 0) {          /* r even -> sign is negative */
            return -s;
        } else {                     /* r odd -> sign is positive */
            return s;
        }
    }

    /* Position r: coefficient of x^r = (-1)^r */
    return ( (r & 1) == 0 ? (seq_t)1 : (seq_t)-1 );
}
