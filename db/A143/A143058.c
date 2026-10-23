/*
 * A143058  a(n) = (n^3 + 18*n^2 + 17*n + 6)/6.
 * https://oeis.org/A143058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 18*n^2 + 17*n + 6)/6
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 + (seq_t)18 * n2 + (seq_t)17 * nn + (seq_t)6;
    return num / (seq_t)6;
}
