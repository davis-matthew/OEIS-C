/*
 * A006918  a(n) = binomial(n+3, 3)/4 for odd n, n*(n+2)*(n+4)/24 for even n.
 * https://oeis.org/A006918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(floor((n+4)/2), 3) + binomial(floor((n+5)/2), 3)
 *            [Eke, shifted]
 * Range:     u8 n <= 16, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t c3(idx_t m) {
    idx_t x, y, z;
    if (m < 3) return (seq_t)0;
    x = m; y = m - 1; z = m - 2;
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    if (x % 2 == 0) x /= 2;
    else y /= 2;
    return (seq_t)x * (seq_t)y * (seq_t)z;
}

seq_t a(idx_t n) {
    return c3((n + 4) / 2) + c3((n + 5) / 2);
}
