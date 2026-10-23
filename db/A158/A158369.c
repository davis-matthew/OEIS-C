/*
 * A158369  a(n) = 576*n^2 + 2*n.
 * https://oeis.org/A158369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 576*n^2 + 2*n * OEIS offset: 1 (a(1) = 578)
 * Range:     u8 none, u16 n <= 10, u32 n <= 2730, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t result = N * N;          /* n^2 */
    result *= (seq_t)576;          /* 576*n^2 */
    result += N * (seq_t)2;        /* + 2*n */
    return result;
}
