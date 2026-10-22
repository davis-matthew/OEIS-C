/*
 * A004054  Expansion of (1-x)/((1+x)*(1-2*x)*(1-3*x)).
 * https://oeis.org/A004054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3), a(0) = 1, a(1) = 3, a(2) =
 *            11
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1, y = 3, z = 11; /* a(k), a(k+1), a(k+2) */
    idx_t k;
    if (n == 0) return x;
    if (n == 1) return y;
    if (n == 2) return z;
    for (k = 3; k <= n; k++) {
        seq_t w = (seq_t)4 * z - y;
        w -= (seq_t)6 * x;
        x = y;
        y = z;
        z = w;
    }
    return z;
}
