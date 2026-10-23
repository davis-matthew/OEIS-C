/*
 * A027850  a(n) = (n+1)*(14*n^3+13*n^2+6*n+1).
 * https://oeis.org/A027850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(14*n^3 + 13*n^2 + 6*n + 1)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 131, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to seq_t */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */

    /* compute 14*n^3 + 13*n^2 + 6*n + 1 */
    seq_t poly = (seq_t)14 * n3
               + (seq_t)13 * n2
               + (seq_t)6  * nn
               + (seq_t)1;

    /* final result (n+1) * poly */
    return (nn + (seq_t)1) * poly;
}
