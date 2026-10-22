/*
 * A002282  a(n) = 8*(10^n - 1)/9.
 * https://oeis.org/A002282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 8*(10^n - 1)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power_of_10 = 1;
    for (idx_t i = 0; i < n; i++) {
        power_of_10 *= 10;
    }
    return 8 * ((power_of_10 - 1) / 9);
}
