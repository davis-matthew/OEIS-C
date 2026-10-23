/*
 * A128153  The number of regular pentagons found by constructing n
 *          equally-spaced points on each side of the pentagon and drawing
 *          lines parallel to the pentagon sides, as well as lines connecting
 *          vertices.
 * https://oeis.org/A128153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = (10*n^2 + 16*n + 9 - (-1)^n)/4 for n > 0, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t term = 10 * nn * nn + 16 * nn + 9;
    /* Adjust according to (-1)^n: subtract 1 if n even, add 1 if n odd */
    if ((n & 1) == 0) {        /* n is even */
        term -= 1;
    } else {                   /* n is odd */
        term += 1;
    }
    return term / 4;
}
