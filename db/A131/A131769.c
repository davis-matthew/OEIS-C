/*
 * A131769  Number of connected components in the double Bruhat cells for
 *          simple Lie groups of type B_n (or C_n).
 * https://oeis.org/A131769
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 5) * 2^(n-1) for n >= 4; a(2)=8; a(3)=30. Equivalent
 *            recurrence for n >= 6: a(n) = 4 * (a(n-1) - a(n-2)) with
 *            a(4)=72, a(5)=160
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A131769.  The sequence is defined for n >= 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)30;
    if (n == 4) return (seq_t)72;
    if (n == 5) return (seq_t)160;

    /* Use the 4·(a(n-1)-a(n-2)) recurrence for n >= 6. */
    seq_t prev2 = (seq_t)72;   /* a(4) */
    seq_t prev1 = (seq_t)160;  /* a(5) */
    seq_t curr;

    for (idx_t i = 6; i <= n; ++i)
    {
        curr   = 4 * (prev1 - prev2);  /* 4*(a(i-1)-a(i-2)) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
