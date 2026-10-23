/*
 * A156089  Alternating sum of the squares of the first n odd-indexed
 *          Fibonacci numbers.
 * https://oeis.org/A156089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies the recurrence a(n) = -7*a(n-1) + 7*a(n-3) +
 *            a(n-4) with initial values a(0)=0, a(1)=-1, a(2)=3, a(3)=-22
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)-1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)-22;

    /* Run the recurrence iteratively, using only the four previous terms. */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)-1;  /* a(n-3) */
    seq_t a2 = (seq_t)3;   /* a(n-2) */
    seq_t a3 = (seq_t)-22; /* a(n-1) */
    seq_t an;             /* a(n) */

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (-7) * a3 + 7 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3; /* a(n) is now stored in a3 */
}
