/*
 * A001056  a(n) = a(n-1)*a(n-2) + 1, a(0) = 1, a(1) = 3.
 * https://oeis.org/A001056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1)*a(n-2) + 1, a(0) = 1, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1, y = 3, t;
    idx_t i;
    if (n == 0) return x;
    for (i = 1; i < n; i++) {
        t = y * x + 1;
        x = y;
        y = t;
    }
    return y;
}
