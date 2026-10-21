/*
 * A221174  a(0)=-4, a(1)=5; thereafter a(n) = 2*a(n-1) + a(n-2).
 * https://oeis.org/A221174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -4, a(1) = 5; a(n) = 2*a(n-1) + a(n-2)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 24, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-4;
    if (n == 1) return (seq_t)5;
    seq_t prev2 = (seq_t)-4;
    seq_t prev1 = (seq_t)5;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = prev1 * 2 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
