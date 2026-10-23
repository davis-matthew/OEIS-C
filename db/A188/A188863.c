/*
 * A188863  Number of n X 6 binary arrays without the pattern 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 243*n - 265 for n>3; a(1)=64, a(2)=239, a(3)=466
 * Range:     u8 n <= 2, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 64;
    if (n == 2) return 239;
    if (n == 3) return 466;
    return (seq_t)n * 243U - 265U;
}
