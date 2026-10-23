/*
 * A171220  a(n) = (2n + 1)*5^n.
 * https://oeis.org/A171220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n + 1)*5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses recurrence a(n) = 10*a(n-1) - 25*a(n-2), a(0)=1, a(1)=15 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)15;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)15;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)10;
        cur -= prev2 * (seq_t)25;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
