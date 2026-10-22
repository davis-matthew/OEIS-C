/*
 * A007486  a(n) = a(n-1) + a(n-2) + a(n-3).
 * https://oeis.org/A007486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(1) = 1, a(2) = 3, a(3) = 4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1, y = 3, z = 4;
    if (n <= 1) return x;
    if (n == 2) return y;
    if (n == 3) return z;
    for (idx_t i = 4; i <= n; i++) {
        seq_t t = x + y + z;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
