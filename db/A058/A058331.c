/*
 * A058331  a(n) = 2*n^2 + 1.
 * https://oeis.org/A058331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 1
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;          /* convert index to seq_t first */
    seq_t res = nn * nn;   /* n^2 */
    res = res * ((seq_t)2) + 1;  /* 2*n^2 + 1 */
    return res;
}
