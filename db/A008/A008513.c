/*
 * A008513  Number of points on surface of 6-dimensional cube.
 * https://oeis.org/A008513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^5 + 40*n^3 + 12*n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 51, u64 n <= 4338,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to seq_t to keep all arithmetic in the
       same (possibly arbitrary‑precision) type. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n5 = n3 * n2;          /* n^5 */
    return (seq_t)12 * n5 + (seq_t)40 * n3 + (seq_t)12 * nn;
}
