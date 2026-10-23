/*
 * A138288  a(n) = A054320(n) - A001078(n).
 * https://oeis.org/A138288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2), with a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* a(-1) = a(0) = 1 */
    seq_t prev2 = (seq_t)1;   /* a(-1) */
    seq_t prev1 = (seq_t)1;   /* a(0)  */
    seq_t cur;

    /* Compute up to a(n) using the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        cur = (seq_t)10 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
