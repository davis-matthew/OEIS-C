/*
 * A003452  Number of nonequivalent dissections of an n-gon into 3 polygons by
 *          nonintersecting diagonals rooted at a cell up to rotation and
 *          reflection.
 * https://oeis.org/A003452
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(n^2-8)/8 for even n, a(n) = (n-4)*(n^2-9)/8 for
 *            odd n, i.e. a(n) = (n-4)*(n^2 - 8 - (n mod 2))/8
 * Range:     u8 n <= 14, u16 n <= 82, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) return 0;
    seq_t N = (seq_t)n;
    seq_t q = N * N - (seq_t)8 - (seq_t)(n & 1);
    return (N - (seq_t)4) * q / (seq_t)8;
}
