/*
 * A037789  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0,1.
 * https://oeis.org/A037789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + 8*a(n-3) - 7*a(n-4), initial
 *            values: a(1)=3, a(2)=23, a(3)=161, a(4)=1128
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)3;
    if (n == (idx_t)2) return (seq_t)23;
    if (n == (idx_t)3) return (seq_t)161;
    if (n == (idx_t)4) return (seq_t)1128;

    seq_t a1 = (seq_t)3,  a2 = (seq_t)23,
           a3 = (seq_t)161, a4 = (seq_t)1128, an = (seq_t)0;
    idx_t i;
    for (i = (idx_t)5; i <= n; ++i)
    {
        an = 8*a4 - 8*a3 + 8*a2 - 7*a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
