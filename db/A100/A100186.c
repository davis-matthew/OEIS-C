/*
 * A100186  Structured heptagonal anti-diamond numbers (vertex structure 7).
 * https://oeis.org/A100186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n^3 - 24*n^2 + 8*n)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 1053, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* OEIS offset is 1, but for n < 1 we return 0 (undefined). */
    if (n == 0) return 0;
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t val = 22 * n3;
    val -= 24 * n2;
    val += 8 * N;
    return val / 6;
}
