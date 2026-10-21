/*
 * A059826  a(n) = (n^2 - n + 1)*(n^2 + n + 1).
 * https://oeis.org/A059826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 1) * (n^2 + n + 1) = n^4 + n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;      /* convert index to sequence type */
    seq_t nsq  = sn * sn;       /* n^2 */
    seq_t left  = nsq - sn + (seq_t)1;   /* n^2 - n + 1 */
    seq_t right = nsq + sn + (seq_t)1;   /* n^2 + n + 1 */
    return left * right;        /* product gives the desired term */
}
