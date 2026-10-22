/*
 * A001169  Number of board-pile polyominoes with n cells.
 * https://oeis.org/A001169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3) for n >= 5, a(2) = 2, a(3)
 *            = 6, a(4) = 19 (a(1) = 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x, y, z, t;
    idx_t i;
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)19;
    x = (seq_t)2;   /* a(i-3) */
    y = (seq_t)6;   /* a(i-2) */
    z = (seq_t)19;  /* a(i-1) */
    for (i = 5; i <= n; i++) {
        t = z * 5 + x * 4 - y * 7;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
