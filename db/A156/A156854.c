/*
 * A156854  a(n) = 2025*n^2 - 3401*n + 1428.
 * https://oeis.org/A156854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2025*n^2 - 3401*n + 1428
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 1457, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return 2025 * N * N - 3401 * N + 1428;
}
