/*
 * A101860  a(n) = (3+n)*(2 + 33*n + n^2)/6.
 * https://oeis.org/A101860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*(n^2 + 33*n + 2) / 6
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t part1 = N + (seq_t)3;                     /* n + 3 */
    seq_t part2 = N * N + (seq_t)33 * N + (seq_t)2; /* n^2 + 33n + 2 */
    seq_t num   = part1 * part2;                    /* (n+3)*(n^2+33n+2) */
    return num / (seq_t)6;                          /* divide by 6 */
}
