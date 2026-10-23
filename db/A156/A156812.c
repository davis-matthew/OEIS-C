/*
 * A156812  a(n) = 225*n^2 - 199*n + 44.
 * https://oeis.org/A156812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*n^2 - 199*n + 44
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 4369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n;
    return 225*t*t - 199*t + 44;
}
