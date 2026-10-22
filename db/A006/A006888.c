/*
 * A006888  a(n) = a(n-1) + a(n-2)*a(n-3) for n > 2 with a(0) = a(1) = a(2) =
 *          1.
 * https://oeis.org/A006888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2)*a(n-3), a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 13, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1, y = 1, z = 1; /* a(i-3), a(i-2), a(i-1) */
    idx_t i;
    if (n < 3) {
        return z;
    }
    for (i = 3; i <= n; i++) {
        seq_t t = z + y * x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
