/*
 * A099868  a(n) = 5*a(n-1) - a(n-2), a(0) = 3, a(1) = 25.
 * https://oeis.org/A099868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2), a(0) = 3, a(1) = 25
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)25;

    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)25;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)5 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
