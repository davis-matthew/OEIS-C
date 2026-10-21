/*
 * A212031  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any element at a city block distance of two, and containing the
 *          value n(n+1)/2-2.
 * https://oeis.org/A212031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2*n^3 - 17*n^2 + 38*n - 16) / 8 for n > 1
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0 */

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t g = n4
            + (seq_t)2 * n3
            - (seq_t)17 * n2
            + (seq_t)38 * nn
            - (seq_t)16;

    return g / 8;
}
