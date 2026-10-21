/*
 * A067724  a(n) = 5*n^2 + 10*n.
 * https://oeis.org/A067724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 10*n
 * Range:     u8 n <= 6, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* convert index to seq_t */
    return 5 * nn * (nn + 2);
}
