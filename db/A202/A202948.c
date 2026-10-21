/*
 * A202948  a(n+1) = 3*A136016*a(n).
 * https://oeis.org/A202948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+1) = 3*(9*n^2-1)*a(n), a(1) = -3
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = -(seq_t)3;            /* a(1) */
    if (n <= 1) return a_val;           /* for offset 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t k = (seq_t)(i - 1);                                          /* k = n-1 */
        seq_t factor = (seq_t)3 * ((seq_t)9 * k * k - (seq_t)1);            /* 3*(9k^2-1) */
        a_val *= factor;
    }
    return a_val;
}
