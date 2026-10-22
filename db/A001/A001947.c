/*
 * A001947  a(n) = Lucas(5*n+2).
 * https://oeis.org/A001947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + a(n-2), a(0) = 3, a(1) = 29
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 3, y = 29;
    idx_t i;
    if (n == 0) return x;
    for (i = 1; i < n; i++) {
        seq_t t = y * 11 + x;
        x = y;
        y = t;
    }
    return y;
}
