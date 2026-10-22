/*
 * A001566  a(0) = 3; thereafter, a(n) = a(n-1)^2 - 2.
 * https://oeis.org/A001566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = a(n-1)^2 - 2, a(0) = 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = (seq_t)3;
    for (idx_t i = 0; i < n; i++) {
        val = val * val - 2;
    }
    return val;
}
