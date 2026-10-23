/*
 * A190970  a(n) = 5*a(n-1) - 9*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 5*a(n-1) - 9*a(n-2)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 21, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 1;   /* a(n-1) */
    seq_t cur   = 0;   /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur = 5 * prev1 - 9 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
