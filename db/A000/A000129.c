/*
 * A000129  Pell numbers: a(0) = 0, a(1) = 1; for n > 1, a(n) = 2*a(n-1) +
 *          a(n-2).
 * https://oeis.org/A000129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, cofr, nice, frac, changed
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 0, y = 1, t;
    idx_t i;
    for (i = 0; i < n; i++) {
        t = y;
        y = 2 * y + x;
        x = t;
    }
    return x;
}
