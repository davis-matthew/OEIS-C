/*
 * A072263  a(n) = 3*a(n-1) + 5*a(n-2), with a(0)=2, a(1)=3.
 * https://oeis.org/A072263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2), a(0) = 2, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;

    seq_t a0 = (seq_t)2;      /* a(0) */
    seq_t a1 = (seq_t)3;      /* a(1) */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i)
    {
        a2 = (seq_t)3 * a1 + (seq_t)5 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;                /* a(n) */
}
