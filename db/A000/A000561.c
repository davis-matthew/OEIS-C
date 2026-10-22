/*
 * A000561  Number of discordant permutations.
 * https://oeis.org/A000561
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n^2 - 45*n + 58)/2
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 829, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * (9 * nn * nn - 45 * nn + 58) / 2;
}
