/*
 * A000533  a(0)=1; a(n) = 10^n + 1, n >= 1.
 * https://oeis.org/A000533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 1 - 0^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = 1;
    for (idx_t i = 0; i < n; i++) {
        power *= 10;
    }
    return power + 1 - (n == 0 ? 1 : 0);
}
