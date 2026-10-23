/*
 * A100702  Number of layers of dough separated by butter in successive
 *          foldings of croissant dough.
 * https://oeis.org/A100702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=3*a(n-1)-2 for n>1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t cur = 3;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = cur * 3 - 2;         /* a(i) = 3*a(i-1)-2 */
    }
    return cur;
}
