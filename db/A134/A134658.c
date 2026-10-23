/*
 * A134658  Triangle read by rows, giving coefficients of extended Jacobsthal
 *          recurrence.
 * https://oeis.org/A134658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = triangle read by rows. For n ≥ 1 let the row index r be
 *            the unique number with r(r+1)/2 ≤ n < (r+1)(r+2)/2. Let k = n -
 *            r(r+1)/2 (0 ≤ k ≤ r). Then: a(0) = 3, a(r,k) = 2 if k==r, a(r,k)
 *            = (-1)^k * C(r, k+1) otherwise
 * Range:     i8 n <= 37, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: binomial coefficient C(n,k) for 0 ≤ k ≤ n. */
static seq_t binomial(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find the row r and its starting index.  cumulative holds the
       number of terms before row r. */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (cumulative + (r + 1u) <= n) {
        cumulative += (r + 1u);
        ++r;
    }
    idx_t k = n - cumulative;   /* position inside the row */

    if (r == 0u) {
        return (seq_t)3;
    }
    if (k == r) {
        return (seq_t)2;
    }

    seq_t value = binomial(r, k + 1u);
    if (k & 1u) {
        value = -value;
    }
    return value;
}
