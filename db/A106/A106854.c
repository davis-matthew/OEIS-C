/*
 * A106854  Expansion of 1/(1-x*(1-5*x)).
 * https://oeis.org/A106854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1) - 5*a(n-2)
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 55, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t curr  = 0;          /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev1 - (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
