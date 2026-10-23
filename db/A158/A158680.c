/*
 * A158680  a(n) = 62*n^2 - 1.
 * https://oeis.org/A158680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 62*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 8323, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Offset is 1; for n < 1 the behavior is unspecified */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;          /* n^2   */
    val *= (seq_t)62;             /* 62*n^2 */
    val -= (seq_t)1;              /* -1    */
    return val;
}
