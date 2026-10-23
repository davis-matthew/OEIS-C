/*
 * A037255  For n weights, number of combinations when limited to two weights
 *          per pan.
 * https://oeis.org/A037255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 - 2*n^3 + 7*n^2 + 2*n) / 8
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 265, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */
    seq_t N4 = N3 * N;         /* n^4 */

    seq_t num = N4 - (seq_t)2 * N3 + (seq_t)7 * N2 + (seq_t)2 * N;
    return num / (seq_t)8;
}
