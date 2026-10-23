/*
 * A033577  a(n) = (3*n+1)*(4*n+1).
 * https://oeis.org/A033577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*(4*n+1)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = sn * 3 + 1;
    seq_t t2 = sn * 4 + 1;
    return t1 * t2;
}
