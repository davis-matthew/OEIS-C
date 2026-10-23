/*
 * A144449  a(n) = 4*(4 + 9*n^2 + 15*n).
 * https://oeis.org/A144449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(4 + 9*n^2 + 15*n)
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;        /* cast index to seq_t for exact arithmetic */
    seq_t val = 16;             /* 4 * 4 */
    val += 36 * nn * nn;        /* 4 * 9 * n^2 = 36 * n^2 */
    val += 60 * nn;             /* 4 * 15 * n = 60 * n */
    return val;
}
