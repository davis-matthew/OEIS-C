/*
 * A175812  Partial sums of ceiling(n^2/6).
 * https://oeis.org/A175812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(2*n^2 + n + 17)/36)
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 2391, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;          /* cast index to seq_t early */
    seq_t term = 2 * sn * sn + sn + 17;   /* 2*n^2 + n + 17  */
    seq_t n1   = sn + 1;                 /* n+1             */
    return (n1 * term) / 36;              /* floor by truncating division */
}
