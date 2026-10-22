/*
 * A006592  a(n) = 10*n^3 - 6*n^2.
 * https://oeis.org/A006592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*n^3 - 6*n^2
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 754, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    return n2 * (10 * nn - 6);
}
