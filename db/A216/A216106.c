/*
 * A216106  The Wiener index of the tetrameric 1,3-adamantane TA(n) (see the
 *          Fath-Tabar et al. reference).
 * https://oeis.org/A216106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*n^3 + 80*n^2 - 34*n
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 440, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return (seq_t)50 * N * N * N + (seq_t)80 * N * N - (seq_t)34 * N;
}
