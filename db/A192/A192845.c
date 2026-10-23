/*
 * A192845  Molecular topological indices of the sun graphs.
 * https://oeis.org/A192845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(-3 + 3*n + n^2)
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t first to avoid overflow in intermediate calculations */
    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn;            /* n^2 */
    temp = temp + 3 * nn;            /* n^2 + 3*n */
    temp = temp - 3;                 /* n^2 + 3*n - 3 */
    return 4 * nn * temp;           /* 4*n*(n^2 + 3*n - 3) */
}
