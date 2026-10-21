/*
 * A087453  a(n) = S(n,5), where S(n,m) = Sum_{k=0..n}
 *          binomial(n,k)*Fibonacci(m*k).
 * https://oeis.org/A087453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 11*a(n-2), a(0) = 0, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 5;

    seq_t prev2 = 0;            /* a(n-2) */
    seq_t prev1 = 5;            /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)13 * prev1 - (seq_t)11 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
