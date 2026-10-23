/*
 * A157110  a(n) = 1681*n^2 - 2606*n + 1010.
 * https://oeis.org/A157110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1681*n^2 - 2606*n + 1010
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 1599, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert the index to seq_t */
    seq_t t  = nn * nn;           /* n^2  */
    t       = 1681 * t;           /* 1681*n^2 */
    t       = t - 2606 * nn;      /* subtract 2606*n */
    t       = t + 1010;           /* add constant 1010 */
    return t;
}
