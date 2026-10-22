/*
 * A002041  Expansion of x/((1-x)(1-4x^2)(1-5x)).
 * https://oeis.org/A002041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 24*a(n-3) + 20*a(n-4), a(0) = 0, a(1)
 *            = 1, a(2) = 6, a(3) = 35
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w = 0, x = 1, y = 6, z = 35; /* a(0), a(1), a(2), a(3) */
    if (n == 0) return w;
    if (n == 1) return x;
    if (n == 2) return y;
    if (n == 3) return z;
    for (idx_t k = 4; k <= n; k++) {
        seq_t pos = z * 6 + w * 20;
        seq_t neg = y + x * 24;
        seq_t t = pos - neg;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
