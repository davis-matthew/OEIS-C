/*
 * A188862  Number of n X 5 binary arrays without the pattern 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 81*n - 65 for n > 2; a(1) = 32, a(2) = 99
 * Range:     u8 n <= 3, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)99;
    seq_t nn = (seq_t)n;
    return 81 * nn - 65;
}
