/*
 * A158413  a(n) = 961*n^2 + 2*n.
 * https://oeis.org/A158413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 961*n^2 + 2*n
 * Range:     u8 none, u16 n <= 8, u32 n <= 2114, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for n < 1, return 0 */
    seq_t tn = (seq_t)n;
    seq_t nn = tn * tn;                    /* n^2 */
    return 961 * nn + 2 * tn;              /* 961*n^2 + 2*n */
}
