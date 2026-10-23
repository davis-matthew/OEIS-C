/*
 * A047732  First differences are A005563.
 * https://oeis.org/A047732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 9*n^2 + 7*n + 6)/6
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* Convert index to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t num = 2 * n3 + 9 * n2 + 7 * sn + 6; /* numerator */
    return num / 6;               /* integer division gives exact result */
}
