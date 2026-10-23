/*
 * A158127  a(n) = 100*n^2 + 2*n.
 * https://oeis.org/A158127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 + 2*n (offset 1)
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 6553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n < 1 is outside the defined offset */
    seq_t nn = (seq_t)n;           /* convert index to the sequence type */
    return nn * (nn * (seq_t)100 + (seq_t)2);
}
