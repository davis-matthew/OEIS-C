/*
 * A229418  Number of 4X4 0..n zero-diagonal arrays with corresponding row and
 *          column sums equal.
 * https://oeis.org/A229418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (87*n^9 + 783*n^8 + 3244*n^7 + 8092*n^6 + 13447*n^5 +
 *            15547*n^4 + 12646*n^3 + 7078*n^2 + 2496*n + 420) / 420
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;          /* cast index to seq_t safely */
    seq_t r = 87;                /* coefficient of n^9 */
    r = r * N + 783;            /* add coefficient of n^8 */
    r = r * N + 3244;           /* add coefficient of n^7 */
    r = r * N + 8092;           /* add coefficient of n^6 */
    r = r * N + 13447;          /* add coefficient of n^5 */
    r = r * N + 15547;          /* add coefficient of n^4 */
    r = r * N + 12646;          /* add coefficient of n^3 */
    r = r * N + 7078;           /* add coefficient of n^2 */
    r = r * N + 2496;           /* add coefficient of n^1 */
    r = r * N + 420;            /* add constant term */
    return r / 420;             /* final division by 420 yields integer result */
}
