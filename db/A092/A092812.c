/*
 * A092812  Number of closed walks of length 2*n on the 4-cube.
 * https://oeis.org/A092812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=40, a(n) = 20*a(n-1) - 64*a(n-2) for n >= 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)40;

    /* Use the linear recurrence for n >= 3. */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t prev1 = (seq_t)40;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)20 - prev2 * (seq_t)64;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
