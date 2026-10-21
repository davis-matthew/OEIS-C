/*
 * A087281  a(n) = Lucas(7*n).
 * https://oeis.org/A087281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 29*a(n-1) + a(n-2), with a(0)=2, a(1)=29 (Lucas(7*n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)29;

    /* Iteratively compute the sequence using the recurrence. */
    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)29;  /* a(n-1) */
    seq_t cur;                /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur   = (seq_t)29 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}
