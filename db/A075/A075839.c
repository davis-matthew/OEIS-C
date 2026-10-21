/*
 * A075839  Numbers k such that 11*k^2 - 2 is a square.
 * https://oeis.org/A075839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - a(n-2), a(1)=1, a(2)=19
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)19;

    seq_t prev2 = (seq_t)1;      /* a(1)   */
    seq_t prev1 = (seq_t)19;     /* a(2)   */
    seq_t cur;

    /* iterate from 3 to n */
    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)20 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
