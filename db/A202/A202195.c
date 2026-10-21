/*
 * A202195  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n+3)*(n+2)^2
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n2   = n + 2;      /* n+2 */
    seq_t n2sq = n2 * n2;    /* (n+2)^2 */
    seq_t n3   = n + 3;      /* n+3 */
    return 3 * n3 * n2sq;    /* 3*(n+3)*(n+2)^2 */
}
