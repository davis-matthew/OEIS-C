/*
 * A002248  Number of points on y^2 + xy = x^3 + x^2 + x over GF(2^n).
 * https://oeis.org/A002248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 4*a(n-4), a(1) = 2, a(2)
 *            = 8, a(3) = 14, a(4) = 16
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t w = 2, x = 8, y = 14, z = 16; /* a(i-3), a(i-2), a(i-1), a(i) */
    idx_t i;

    if (n == 1) return w;
    if (n == 2) return x;
    if (n == 3) return y;
    if (n == 4) return z;

    for (i = 5; i <= n; i++) {
        /* keep every intermediate value nonnegative */
        seq_t pos = 4 * z + 8 * x;
        seq_t neg = 7 * y + 4 * w;
        seq_t next = pos - neg;
        w = x;
        x = y;
        y = z;
        z = next;
    }
    return z;
}
