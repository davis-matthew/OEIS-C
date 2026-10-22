/*
 * A002278  a(n) = 4*(10^n - 1)/9.
 * https://oeis.org/A002278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*10^(n-1), a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power_of_10 = 1;
    for (idx_t i = 0; i < n; i++) {
        result = result + 4 * power_of_10;
        power_of_10 = power_of_10 * 10;
    }
    return result;
}
