/*
 * A163943  Third left hand column of triangle A163940.
 * https://oeis.org/A163943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 45*n^2 + 22*n^3 + 3*n^4)/24
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 322, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;
    seq_t num = (seq_t)3 * n4 + (seq_t)22 * n3 + (seq_t)45 * n2 + (seq_t)2 * sn;
    return num / (seq_t)24;
}
