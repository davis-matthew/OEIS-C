/*
 * A007909  Expansion of (1-x)/(1-2*x+x^2-2*x^3).
 * https://oeis.org/A007909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3), a(0) = 1, a(1) = 1, a(2) =
 *            1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1, y = 1, z = 1; /* a(i-3), a(i-2), a(i-1) */
    idx_t i;
    if (n < 3) return (seq_t)1;
    for (i = 3; i <= n; i++) {
        seq_t t = z + z + x + x;
        t -= y;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
