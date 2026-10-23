/*
 * A015461  q-Fibonacci numbers for q=4, scaling a(n-2).
 * https://oeis.org/A015461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4^(n-2) * a(n-2) * Initial values: a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Iterate using a simple linear recurrence. */
    seq_t prev2 = (seq_t)0;        /* a(i-2)   */
    seq_t prev1 = (seq_t)1;        /* a(i-1)   */
    seq_t pow4  = (seq_t)1;        /* 4^(i-2)  for the next iteration */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = prev1 + pow4 * prev2;  /* a(i) = a(i-1) + 4^(i-2)*a(i-2) */
        prev2 = prev1;
        prev1 = term;
        pow4 *= (seq_t)4;                       /* advance 4^(i-1-2) */
    }

    return prev1;   /* a(n) */
}
