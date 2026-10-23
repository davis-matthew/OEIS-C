/*
 * A158886  a(n) = (n+1)^n * n! * C(1/(n+1), n).
 * https://oeis.org/A158886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Π_{k=0}^{n-1} (1 - k*(n+1)), a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t res = 1;
    seq_t n1 = (seq_t)n + 1;         /* (n+1) as seq_t to avoid overflow on 64‑bit builds */
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = 1 - ((seq_t)k * n1);   /* term = 1 - k*(n+1) */
        res *= term;                       /* cumulative product */
    }
    return res;
}
