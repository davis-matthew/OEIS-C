/*
 * A113772  Determinant of the 2 X 2 matrices where the first column is
 *          consecutive triangular numbers and the second column is the
 *          corresponding consecutive Fibonacci numbers.
 * https://oeis.org/A113772
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = t(n‑1)*f(n) - t(n)*f(n‑1), where t(k) = k(k+1)/2 and
 *            f(1)=f(2)=1, f(k)=f(k-1)+f(k-2)
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 35, i64 n <= 77, i128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* defined only for n >= 2 according to OEIS offset */
    if (n < 2)
        return 0;

    /* triangular numbers */
    seq_t tn     = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t tnm1   = (seq_t)(n - 1) * (seq_t)n / 2;

    /* Fibonacci numbers */
    seq_t f_prev = 1;   /* f(1)   */
    seq_t f_curr = 1;   /* f(2)   */

    if (n >= 3) {
        for (idx_t i = 3; i <= n; ++i) {
            seq_t next  = f_prev + f_curr;
            f_prev      = f_curr;
            f_curr      = next;
        }
    }
    /* At this point:
       f_curr = f(n)
       f_prev = f(n-1)   */
    seq_t fn   = f_curr;
    seq_t fnm1 = f_prev;

    /* determinant */
    seq_t term1 = tnm1 * fn;
    seq_t term2 = tn   * fnm1;

    return term1 - term2;
}
