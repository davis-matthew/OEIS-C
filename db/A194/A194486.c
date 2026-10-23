/*
 * A194486  Number of ways to arrange 2 nonattacking knights on the lower
 *          triangle of an n X n board.
 * https://oeis.org/A194486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2*n^3 - 17*n^2 + 62*n - 64)/8 for n>1, a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */
    seq_t num = n4 + (seq_t)2 * n3 - (seq_t)17 * n2 + (seq_t)62 * N - (seq_t)64;
    return num / 8;
}
