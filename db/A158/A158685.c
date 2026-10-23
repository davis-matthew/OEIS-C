/*
 * A158685  a(n) = 32*(32*n^2 + 1).
 * https://oeis.org/A158685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*(32*n^2 + 1)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;          /* promote index to seq_t */
    seq_t res = nn * nn;   /* n^2 */
    res = res * 32 + 1;    /* 32*n^2 + 1 */
    res *= 32;             /* 32*(32*n^2 + 1) */
    return res;
}
