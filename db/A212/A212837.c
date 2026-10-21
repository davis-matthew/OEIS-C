/*
 * A212837  Number of 0..n arrays of length 4 with 0 never adjacent to n.
 * https://oeis.org/A212837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 - 4*n + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t result = nn * nn * nn * nn;       /* n^4 */
    result += 4 * nn * nn * nn;             /* + 4*n^3 */
    result -= 4 * nn;                       /* - 4*n */
    result += 1;                            /* + 1 */
    return result;
}
