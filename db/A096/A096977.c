/*
 * A096977  a(n) = 4*a(n-1) + 3*a(n-2) - 14*a(n-3) + 8*a(n-4).
 * https://oeis.org/A096977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) - 14*a(n-3) + 8*a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)11;

    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)2;   /* a(n-2) */
    seq_t a3 = (seq_t)11;  /* a(n-1) */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i)
    {
        a4 = 4 * a3 + 3 * a2 - 14 * a1 + 8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
