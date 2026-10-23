/*
 * A016952  a(n) = (6*n + 3)^8.
 * https://oeis.org/A016952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 42, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t base = 6 * m + 3;     /* compute 6*n + 3 */
    seq_t b2 = base * base;    /* base^2 */
    seq_t b4 = b2 * b2;        /* base^4 */
    seq_t b8 = b4 * b4;        /* base^8 */
    return b8;
}
