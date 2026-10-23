/*
 * A157616  On an n X n board, a(n) is the maximal number of squares covered
 *          by a self-avoiding path that starts from a corner and is made of
 *          alternated vertical and horizontal unitary steps.
 * https://oeis.org/A157616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n^2 - n + 2 if n is even, a(n) = n^2
 *            - 2n + 3 if n is odd
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return 1;
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {        /* even n */
        return nn * nn - nn + 2;
    } else {                   /* odd n > 1 */
        return nn * nn - 2 * nn + 3;
    }
}
