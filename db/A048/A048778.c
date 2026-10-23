/*
 * A048778  First partial sums of A048745; second partial sums of A048654.
 * https://oeis.org/A048778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 3*n + 1, a(0) = 1, a(1) = 6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
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
        return (seq_t)6;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)6;          /* a(1) */
    seq_t an = a1;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)2 * a1 + a0 + (seq_t)3 * (seq_t)i + (seq_t)1;
        a0 = a1;
        a1 = an;
    }
    return an;
}
