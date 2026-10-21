/*
 * A067725  a(n) = 3*n^2 + 6*n.
 * https://oeis.org/A067725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 6*n
 * Range:     u8 n <= 8, u16 n <= 146, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 3 * nn * (nn + 2);
}
