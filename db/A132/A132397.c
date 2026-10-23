/*
 * A132397  Second trisection of A024494.
 * https://oeis.org/A132397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=10, a(n)=7*a(n-1)+8*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)10;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)10;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = a1 * 7 + a0 * 8;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
