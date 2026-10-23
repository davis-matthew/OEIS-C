/*
 * A033585  a(n) = 2*n*(4*n + 1).
 * https://oeis.org/A033585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(4*n + 1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to seq_t type */
    /* compute the formula without intermediate overflow in most builds */
    return 2 * nn * (4 * nn + 1);
}
