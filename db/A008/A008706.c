/*
 * A008706  Coordination sequence for 3.3.3.4.4 planar net.
 * https://oeis.org/A008706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0^n + 5*n (=> a(0)=1, a(n)=5*n for n>0)
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return (seq_t)5 * (seq_t)n;
}
