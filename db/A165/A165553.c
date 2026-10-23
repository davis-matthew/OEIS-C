/*
 * A165553  a(n) = (3/2)*(1+(-3)^(n-1)).
 * https://oeis.org/A165553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (3/2)*(1+(-3)^(n-1)) Recurrence: a(0) = 1, a(1) = 3, a(n)
 *            = 3*a(n-2) - 2*a(n-1)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 3;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = 3 * prev2 - 2 * prev1;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
