/*
 * A115341  a(n) = abs(A154879(n+1)).
 * https://oeis.org/A115341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 2*a(n-2) Initial values: a(0) = 2, a(1) = 4,
 *            a(2) = 0 (for n >= 3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)0;

    /* Compute using the recurrence a(n) = a(n-1) + 2*a(n-2). */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t prev1 = (seq_t)0;   /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur   = prev1 + 2 * prev2;  /* a(i) = a(i-1) + 2*a(i-2) */
        prev2 = prev1;              /* shift for next step */
        prev1 = cur;
    }
    return prev1;  /* a(n) for n >= 3 */
}
