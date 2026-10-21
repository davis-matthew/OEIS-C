/*
 * A206608  Fibonacci sequence beginning 13, 10.
 * https://oeis.org/A206608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), with a(1)=13, a(2)=10
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A206608 using the recurrence. */
seq_t a(idx_t n)
{
    /* Handle initial terms directly. */
    if (n == (idx_t)1)
        return (seq_t)13;
    if (n == (idx_t)2)
        return (seq_t)10;

    /* Iterate using the recurrence. */
    seq_t prev2 = (seq_t)13;   /* a(1) */
    seq_t prev1 = (seq_t)10;   /* a(2) */
    seq_t cur;

    /* Loop from 3 up to n inclusive. */
    for (idx_t i = (idx_t)3; i <= n; ++i) {
        cur   = prev1 + prev2; /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;         /* shift for next iteration */
        prev1 = cur;
    }
    return prev1;
}
