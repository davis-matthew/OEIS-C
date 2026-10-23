/*
 * A107253  a(n) = n^4 - 15*n + 15.
 * https://oeis.org/A107253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 15*n + 15
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n4  = n2 * n2;
    seq_t res = n4 - (seq_t)15 * nn + (seq_t)15;
    return res;
}
