/*
 * A005267  a(n) = -1 + a(0)*a(1)*...*a(n-1) with a(0) = 3.
 * https://oeis.org/A005267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)^2 + a(n-1) - 1 for n >= 2, a(0) = 3, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v;
    idx_t i;
    if (n == 0) {
        v = 3;
        return v;
    }
    v = 2;
    for (i = 2; i <= n; i++) {
        v = v * v + v - 1;
    }
    return v;
}
