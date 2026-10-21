/*
 * A099368  Twice Chebyshev polynomials of the first kind, T(n,x), evaluated
 *          at x=51/2.
 * https://oeis.org/A099368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 51*a(n-1) - a(n-2), a(0)=2, a(1)=51
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)51;

    /* Iteratively compute the recurrence using only the two most recent terms */
    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)51;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)51 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;  /* a(n) */
}
