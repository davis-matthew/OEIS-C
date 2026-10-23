/*
 * A158591  a(n) = 36*n^2 + 1.
 * https://oeis.org/A158591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;   /* n^2 */
    res = res * (seq_t)36; /* 36 * n^2 */
    return res + (seq_t)1; /* +1 */
}
