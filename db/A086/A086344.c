/*
 * A086344  a(n) = -2*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 0.
 * https://oeis.org/A086344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 4*a(n-2) a(0) = 1, a(1) = 0
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Iteratively compute up to n using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)0;   /* a(n-1) */
    seq_t cur   = 0;          /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)(-2) * prev1 + (seq_t)4 * prev2; /* a(i) = -2*a(i-1) + 4*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
