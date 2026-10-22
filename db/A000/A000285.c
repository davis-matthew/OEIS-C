/*
 * A000285  a(0) = 1, a(1) = 4, and a(n) = a(n-1) + a(n-2) for n >= 2.
 * https://oeis.org/A000285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1, y = 4, t;
    idx_t i;
    if (n == 0) return x;
    for (i = 1; i < n; i++) {
        t = x + y;
        x = y;
        y = t;
    }
    return y;
}
