/*
 * A000803  a(n+3) = a(n+2) + a(n+1) + a(n) - 4.
 * https://oeis.org/A000803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - 4, a(0) = 0, a(1) = 0, a(2) =
 *            8
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 0, y = 0, z = 8;
    idx_t i;
    if (n == 0) return x;
    if (n == 1) return y;
    for (i = 2; i < n; i++) {
        seq_t t = z + y + x - 4;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
