/*
 * A005610  Number of Boolean functions realized by cascades of n gates.
 * https://oeis.org/A005610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 2, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 2;
    for (idx_t i = 2; i <= n; i++) {
        result = 6 * result + 2;
    }
    return result;
}
