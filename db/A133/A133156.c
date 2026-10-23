/*
 * A133156  Irregular triangle read by rows: coefficients of U(n,x), Chebyshev
 *          polynomials of the second kind with exponents in decreasing order.
 * https://oeis.org/A133156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A133156.
    The terms are the coefficients of the Chebyshev polynomials of the second
    kind U(n,x) read by rows.  The coefficient T(r,m) of the term with
    degree (r-2*m) in U(r,x) is

        T(r,m) = (-1)^m * binomial(r-m, m) * 2^(r-2*m),

    where 0 ≤ m ≤ floor(r/2).  The sequence a(n) is obtained by writing all
    rows in order, with each row enumerated from m = 0 upwards.
    -------------------------------------------------------------------------- */

seq_t a(idx_t n) {
    /* Find the row r that contains the n-th term.  Row r has
       floor(r/2)+1 elements.  */
    idx_t r   = 0;
    idx_t cum = 0;                 /* cumulative number of terms up to row r-1 */
    for(;; ++r) {
        idx_t terms = (r / 2) + 1;
        if (cum + terms > n) break;
        cum += terms;
    }

    /* Position of the term inside its row */
    idx_t m      = n - cum;          /* 0 <= m <= floor(r/2) */
    idx_t nm     = r - m;            /* r - m */
    idx_t pw_exp = r - (m << 1);     /* r - 2*m  */

    /* Compute the binomial coefficient C(r-m, m) */
    seq_t bin = 1;
    for (idx_t i = 1; i <= m; ++i) {
        bin = bin * (seq_t)(nm - m + i);
        bin = bin / (seq_t)i;       /* integer division – exact */
    }

    /* Compute 2^(pw_exp) */
    seq_t p2 = 1;
    for (idx_t i = 0; i < pw_exp; ++i) {
        p2 = p2 * (seq_t)2;
    }

    /* Combine, apply the alternating sign and return. */
    seq_t result = bin * p2;
    if (m & 1) result = -result;
    return result;
}
