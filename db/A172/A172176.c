/*
 * A172176  Triangle T(n, k) = 1 + (n + k - n*k)*(2*n - k - n*(n-k)), read by
 *          rows.
 * https://oeis.org/A172176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) equals the triangle T(n, k) read by rows, where T(n, k) = 1
 *            + (n - (n-1)k) * (n - (n-1)(n-k)), 0 ≤ k ≤ n The index n is
 *            flattened so that row r starts at index r(r+1)/2
 * Range:     i8 n <= 20, i16 n <= 238, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first element directly. */
    if (n == 0) return 1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t low = 0;
    idx_t high = 1;
    /* Increase high until it exceeds the required triangular bound. */
    while ((high * (high + 1)) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search for r. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    idx_t r = low;                            /* row index */
    idx_t k = n - (r * (r + 1)) / 2;           /* column index in the row */

    /* Compute T(r, k) using signed arithmetic. */
    seq_t r_s      = (seq_t)r;
    seq_t k_s      = (seq_t)k;
    seq_t r_minus1 = r_s - 1;                 /* may be negative if r == 0 */
    seq_t term_a   = r_s - r_minus1 * k_s;    /* n - (n-1)k       */
    seq_t term_b   = r_s - r_minus1 * ((seq_t)(r - k)); /* n - (n-1)(n-k) */
    return 1 + term_a * term_b;
}
