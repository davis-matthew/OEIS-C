/*
 * A005178  Number of domino tilings of 4 X (n-1) board.
 * https://oeis.org/A005178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + a(n-3) - a(n-4), a(0) = 0, a(1) = 1,
 *            a(2) = 1, a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w = 0, x = 1, y = 1, z = 5;  /* a(i-3), a(i-2), a(i-1), a(i) for i = 3 */
    if (n == 0) return w;
    if (n == 1) return x;
    if (n == 2) return y;
    if (n == 3) return z;
    for (idx_t i = 4; i <= n; i++) {
        seq_t t = z + 5 * y + x;
        t -= w;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
