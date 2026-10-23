/*
 * A008000  Coordination sequence T1 for Zeolite Code ABW and ATN.
 * https://oeis.org/A008000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1, 19*m^2+2, 19*m^2+13m+4, 19*m^2+25m+10} for n=0 or
 *            n=3m+r, r=0,1,2, m>0
 * Range:     u8 n <= 10, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m = n / 3;
    int r = n % 3;
    seq_t mm = (seq_t)m;

    if (r == 0)
        return (seq_t)19 * mm * mm + (seq_t)2;
    else if (r == 1)
        return (seq_t)19 * mm * mm + (seq_t)13 * mm + (seq_t)4;
    else /* r == 2 */
        return (seq_t)19 * mm * mm + (seq_t)25 * mm + (seq_t)10;
}
