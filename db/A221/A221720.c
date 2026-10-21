/*
 * A221720  An avoidance sequence for a pair of tree patterns that is not the
 *          avoidance sequence for any set of permutations.
 * https://oeis.org/A221720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 - 56*n + 147 for n > 6 a(1) = 1, a(2) = 1, a(3) =
 *            2, a(4) = 5, a(5) = 12, a(6) = 26 (Offset 1)
 * Range:     u8 n <= 10, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* small values (n <= 6) */
    static const seq_t init[6] = {1, 1, 2, 5, 12, 26};
    if (n == 0) return 0;          /* behaviour for n<1 does not matter */
    if (n <= 6) return init[n - 1];

    /* polynomial for n > 6 */
    seq_t nn = n;
    seq_t res = 6 * nn * nn;       /* 6*n^2 */
    res -= 56 * nn;                /* -56*n */
    res += 147;                    /* +147 */
    return res;
}
