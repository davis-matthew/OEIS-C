/*
 * A172193  a(n) = 5*n^2 + 31*n + 1.
 * https://oeis.org/A172193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 31*n + 1
 * Range:     u8 n <= 4, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* cast index to sequence type */
    seq_t r = 5 * nn * nn + 31 * nn + 1;
    return r;
}
