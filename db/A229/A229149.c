/*
 * A229149  a(n) = n^6*(4*n+3).
 * https://oeis.org/A229149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(4*n+3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 463, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;        /* n^2 */
    seq_t x3 = x2 * x;       /* n^3 */
    seq_t x4 = x3 * x;       /* n^4 */
    seq_t x5 = x4 * x;       /* n^5 */
    seq_t x6 = x5 * x;       /* n^6 */
    seq_t factor = (seq_t)4 * x + (seq_t)3; /* 4n+3 */
    return x6 * factor;
}
