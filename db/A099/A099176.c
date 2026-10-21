/*
 * A099176  a(n) = 2*a(n-1) + 4*a(n-2) - 4*a(n-3) - 4*a(n-4).
 * https://oeis.org/A099176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 4*a(n-3) - 4*a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=4, a(3)=8
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)4;
    if (n == 3u) return (seq_t)8;

    seq_t a0 = (seq_t)1, a1 = (seq_t)1, a2 = (seq_t)4, a3 = (seq_t)8;
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t tmp1 = a3 * (seq_t)2 + a2 * (seq_t)4;
        seq_t tmp2 = a1 * (seq_t)4 + a0 * (seq_t)4;
        seq_t a4  = tmp1 - tmp2;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
