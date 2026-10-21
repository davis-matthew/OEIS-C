/*
 * A064302  Sixth diagonal of triangle A064094.
 * https://oeis.org/A064302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 4*n + 9*n^2 + 14*n^3 + 14*n^4
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 132, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;          /* cast index to seq_t */
    seq_t m2 = m * m;             /* n^2 */
    seq_t m3 = m2 * m;            /* n^3 */
    seq_t m4 = m3 * m;            /* n^4 */

    return 1 + 4*m + 9*m2 + 14*m3 + 14*m4;
}
