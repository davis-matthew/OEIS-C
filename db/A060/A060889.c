/*
 * A060889  a(n) = n^8 - n^7 + n^5 - n^4 + n^3 - n + 1.
 * https://oeis.org/A060889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 - n^7 + n^5 - n^4 + n^3 - n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;           /* n^2 */
    seq_t N3 = N2 * N;          /* n^3 */
    seq_t N4 = N3 * N;          /* n^4 */
    seq_t N5 = N4 * N;          /* n^5 */
    seq_t N7 = N5 * N2;         /* n^7 = n^5 * n^2 */
    seq_t N8 = N7 * N;          /* n^8 = n^7 * n */

    seq_t res = N8 - N7 + N5 - N4 + N3 - N + (seq_t)1;
    return res;
}
