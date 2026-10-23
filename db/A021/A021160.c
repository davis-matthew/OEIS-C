/*
 * A021160  Decimal expansion of 1/156.
 * https://oeis.org/A021160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0, a(n)={6,4,1,0,2,5}[(n-2)%6] for n>=2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    static const seq_t pat[6] = {6, 4, 1, 0, 2, 5};
    return pat[(n - 2) % 6];
}
