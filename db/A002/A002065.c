/*
 * A002065  a(n+1) = a(n)^2 + a(n) + 1.
 * https://oeis.org/A002065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n+1) = a(n)^2 + a(n) + 1, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = 0;
    for (idx_t i = 0; i < n; i++) {
        val = val * val + val + 1;
    }
    return val;
}
