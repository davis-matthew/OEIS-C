/*
 * A164845  a(n) = (6 + 10*n + 5*n^2 + n^3)/2.
 * https://oeis.org/A164845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6 + 10*n + 5*n^2 + n^3)/2
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1687, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = (seq_t)6
                + (seq_t)10 * nn
                + (seq_t)5 * nn * nn
                + nn * nn * nn;
    return num / (seq_t)2;
}
