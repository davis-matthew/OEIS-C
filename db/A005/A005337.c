/*
 * A005337  Number of ways in which n identical balls can be distributed among
 *          4 boxes in a row such that each pair of adjacent boxes contains at
 *          least 4 balls.
 * https://oeis.org/A005337
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (546 - 169*n + 6*n^2 + n^3)/6
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t result = 546 - 169*nn + 6*nn*nn + nn*nn*nn;
    return result / 6;
}
