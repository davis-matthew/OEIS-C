/*
 * A106731  Expansion of -2*x/(1 - 4*x + 2*x^2).
 * https://oeis.org/A106731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(0)=0, a(1)=-2, a(n)=4*a(n-1)-2*a(n-2) for n>=2
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 35, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-2;
    seq_t prev2 = (seq_t)0;     /* a(0) */
    seq_t prev1 = (seq_t)-2;    /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = 4 * prev1 - 2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
