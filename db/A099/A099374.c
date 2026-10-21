/*
 * A099374  a(n) = A041041(n-1)^2, n >= 1.
 * https://oeis.org/A099374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1) + 101*a(n-2) - a(n-3), n ≥ 3; Initial values:
 *            a(0)=0, a(1)=1, a(2)=100
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)100;

    seq_t prev3 = (seq_t)0;   /* a(n-3)  */
    seq_t prev2 = (seq_t)1;   /* a(n-2)  */
    seq_t prev1 = (seq_t)100; /* a(n-1)  */
    seq_t an = prev1;         /* placeholder */

    /* repeat recurrence until we reach a(n) */
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)101 * prev1 + (seq_t)101 * prev2 - prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = an;
    }
    return an;
}
