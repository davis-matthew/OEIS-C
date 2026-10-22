/*
 * A000078  Tetranacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) for n
 *          >= 4 with a(0) = a(1) = a(2) = 0 and a(3) = 1.
 * https://oeis.org/A000078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), a(0) = a(1) = a(2) =
 *            0, a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 37, u64 n <= 71, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t w = 0, x = 0, y = 0, z = 1, t;
    idx_t i;
    if (n < 3)
        return (seq_t)0;
    for (i = 3; i < n; i++) {
        t = w + x + y + z;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
