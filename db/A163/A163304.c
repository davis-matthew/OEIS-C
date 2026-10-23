/*
 * A163304  a(n) = n^4 + 984*n^3 + 902*n^2 + 394*n + 858.
 * https://oeis.org/A163304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 984*n^3 + 902*n^2 + 394*n + 858
 * Range:     u8 none, u16 n <= 3, u32 n <= 155, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast n first to avoid truncated arithmetic */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t n4 = n3 * sn;           /* n^4 */
    return n4
           + (seq_t)984 * n3
           + (seq_t)902 * n2
           + (seq_t)394 * sn
           + (seq_t)858;
}
