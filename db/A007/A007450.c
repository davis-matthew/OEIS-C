/*
 * A007450  Decimal expansion of 1/17.
 * https://oeis.org/A007450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn, easy
 * Formula:   a(n) = decimal digit of 1/17 at position n, periodic with period
 *            16
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pattern[16] = {0, 5, 8, 8, 2, 3, 5, 2, 9, 4, 1, 1, 7, 6, 4, 7};
    return pattern[n % 16];
}
