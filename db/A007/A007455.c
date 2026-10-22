/*
 * A007455  Number of subsequences of [ 1,...,n ] in which each odd number has
 *          an even neighbor.
 * https://oeis.org/A007455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-4), a(0) = 1, a(1) = 1, a(2) = 3, a(3) =
 *            5
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w = 1, x = 1, y = 3, z = 5; /* a(i-4), a(i-3), a(i-2), a(i-1) */
    idx_t i;
    if (n == 0) return w;
    if (n == 1) return x;
    if (n == 2) return y;
    if (n == 3) return z;
    for (i = 4; i <= n; i++) {
        seq_t t = 3 * y + 2 * w;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
