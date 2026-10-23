/*
 * A190961  a(n) = 3*a(n-1) - 7*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 7*a(n-2); a(0)=0, a(1)=1
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 23, i64 n <= 46, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)1;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur  = (seq_t)3 * prev1 - (seq_t)7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
