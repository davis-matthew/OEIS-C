/*
 * A140679  a(n) = n*(3*n+14).
 * https://oeis.org/A140679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n+14)
 * Range:     u8 n <= 7, u16 n <= 145, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t first */
    seq_t temp = 3 * nn + 14;     /* 3*n + 14 */
    return nn * temp;             /* n * (3*n + 14) */
}
