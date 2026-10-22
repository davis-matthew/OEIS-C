/*
 * A001860  Number of series-reduced planted trees with n+9 nodes and 4
 *          internal nodes.
 * https://oeis.org/A001860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [(8*n^3 + 27*n^2 + 21*n + 9)/18]
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 937, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return (8 * m * m * m + 27 * m * m + 21 * m + 9) / 18;
}
