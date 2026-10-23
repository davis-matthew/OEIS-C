/*
 * A131522  Expansions of p(x,n)=(x^n - 1)*(x^(n + 1) - 1) for n >= 1,
 *          triangle read by rows.
 * https://oeis.org/A131522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, easy, less, sign
 * Formula:   a(n) is the coefficient of x^(e) in (x^r – 1)(x^(r+1) – 1),
 *            where r is the unique integer such that (r−1)(r+2) < n ≤ r^2 +
 *            3r. The coefficients are +1 at exponents 0 and 2r+1, –1 at
 *            exponents r and r+1, 0 otherwise
 * Range:     i8 n <= 118, i16 n <= 118, i32 n <= 118, i64 n <= 118,
 *            i128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* value below the offset is undefined */

    /* Find the smallest r with r^2 + 3r >= n.  For n ≤ 10^4 this
       loop terminates in at most a few dozen iterations. */
    idx_t r = 1;
    while (r * r + 3 * r < n) {
        ++r;
    }

    /* Linear index where row r starts */
    idx_t start = r * r + r - 2;   /* L(r) = (r−1)(r+2) = r^2 + r − 2 */

    /* Position within row (1‑based) */
    idx_t pos = n - start;        /* 1 ≤ pos ≤ 2(r+1) */

    /* Exponent of the corresponding term in the polynomial */
    idx_t exp = pos - 1;           /* 0‑based exponent */

    if (exp == 0 || exp == 2 * r + 1) {
        return (seq_t)1;
    } else if (exp == r || exp == r + 1) {
        return (seq_t)-1;
    } else {
        return (seq_t)0;
    }
}
