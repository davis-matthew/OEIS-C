/*
 * A140056  Triangle read by rows: T(n,k) = [x^k] (-1-x-x^(n-1)) with
 *          T(0,0)=-3.
 * https://oeis.org/A140056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  less, tabf, sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140056: triangle read by rows.
   Row 0:          {-3}
   Row n (n>=1):   rows of coefficients of (-1 - x - x^(n-1)),
   written as a sequence in the order
   ([x^0] + [x^1] + [x^(n-1)]) with missing terms taken as 0.
   The nth global term a(n) (0‑based) is obtained by concatenating all
   rows in order. */

seq_t a(idx_t n) {
    /* Handle the special first row explicitly. */
    if (n == 0)
        return (seq_t)-3;

    /* Find the row `r` that contains the global index `n`. */
    idx_t accum = 1;      /* number of terms up to row 0. */
    idx_t r = 1;          /* current row number (n >= 1). */

    while (1) {
        idx_t len = (r == 1) ? 2 : r; /* length of row r. */
        if (n < accum + len)
            break;
        accum += len;
        ++r;
    }

    /* Position within the located row. */
    idx_t p = n - accum;

    /* Compute the coefficient at position p in row r. */
    seq_t val = 0;
    if (p == 0)           /* constant term */
        val -= 1;
    if (p == 1)           /* coefficient of x */
        val -= 1;
    if (p == r - 1)       /* coefficient of x^(n-1) */
        val -= 1;

    return val;
}
