/*
 * A063809  Spherical growth series for pair of groups, one Gromov hyperbolic,
 *          the other not.
 * https://oeis.org/A063809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 108, a(2) = 2160, a(n) = 20*a(n-1) - 28*a(n-2)
 *            for n >= 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)108;
    if (n == 2)
        return (seq_t)2160;

    seq_t prev2 = (seq_t)108;   /* a(n-2) for n=3 */
    seq_t prev1 = (seq_t)2160;  /* a(n-1) for n=3 */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 20 * prev1 - 28 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
