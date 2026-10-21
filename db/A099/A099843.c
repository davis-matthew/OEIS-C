/*
 * A099843  A transform of the Fibonacci numbers.
 * https://oeis.org/A099843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-1) + a(n-2), a(0) = 1, a(1) = -5
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 30, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)-5;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)-5;  /* a(1) */
    seq_t curr  = 0;

    /* Loop from 2 to n inclusive */
    for (idx_t i = 2; i <= n; ++i)
    {
        curr = - (seq_t)4 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
