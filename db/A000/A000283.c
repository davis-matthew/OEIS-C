/*
 * A000283  a(n) = a(n-1)^2 + a(n-2)^2 for n >= 2 with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A000283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^2 + a(n-2)^2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 0, y = 1, t;
    idx_t i;
    if (n == 0)
        return x;
    for (i = 1; i < n; i++) {
        t = y * y + x * x;
        x = y;
        y = t;
    }
    return y;
}
