/*
 * A066629  a(n) = 2*Fibonacci(n+2) + ((-1)^n - 3)/2.
 * https://oeis.org/A066629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*Fibonacci(n+2) + ((-1)^n - 3)/2 Equivalent recurrence
 *            (starting from a(0)=1, a(1)=2): if n is even: a(n) = a(n-1) +
 *            a(n-2) + 2 if n is odd : a(n) = a(n-1) + a(n-2) + 1
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the known first two terms */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* iterate using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & (idx_t)1) == (idx_t)0)  /* even n */
            cur = prev1 + prev2 + (seq_t)2;
        else                            /* odd n */
            cur = prev1 + prev2 + (seq_t)1;

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
