/*
 * A165491  a(0)=1, a(1)=6, a(n) = 30*a(n-2) - a(n-1).
 * https://oeis.org/A165491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=6, a(n)=30*a(n-2)-a(n-1)
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 12, i64 n <= 25, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;

    seq_t prev2 = 1; /* a(n-2) */
    seq_t prev1 = 6; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = 30 * prev2 - prev1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
