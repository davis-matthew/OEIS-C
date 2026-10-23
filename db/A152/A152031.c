/*
 * A152031  a(n) = n^5 + n^4 + n^3 + n^2 + n.
 * https://oeis.org/A152031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + n^4 + n^3 + n^2 + n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to the value type */
    seq_t s2 = N * N;            /* n^2 */
    seq_t s3 = s2 * N;           /* n^3 */
    seq_t s4 = s3 * N;           /* n^4 */
    seq_t s5 = s4 * N;           /* n^5 */
    return s5 + s4 + s3 + s2 + N;
}
