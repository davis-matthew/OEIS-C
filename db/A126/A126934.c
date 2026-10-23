/*
 * A126934  Define an array by d(m, 0) = 1, d(m, 1) = m; d(m, k) = (m - k + 1)
 *          d(m+1, k-1) - (k-1) (m+1) d(m+2, k-2). Sequence gives d(0,2n).
 * https://oeis.org/A126934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(0) * Π_{k=1}^n (-2*(2k-1)^2); a(0)=1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 1;            /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t v = (seq_t)(2 * k - 1);   /* (2k-1) as seq_t */
        v *= v;                        /* (2k-1)^2 */
        v *= (seq_t)-2;                /* *(-2) */
        ans *= v;                      /* ans *= -2*(2k-1)^2 */
    }
    return ans;
}
