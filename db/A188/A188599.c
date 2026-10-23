/*
 * A188599  Expansion of x/(1-6*x+25*x^2).
 * https://oeis.org/A188599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 25*a(n-2), a(0)=0, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 28, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t cur   = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = 6 * prev1 - 25 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
