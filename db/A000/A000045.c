/*
 * A000045  Fibonacci numbers: F(n) = F(n-1) + F(n-2) with F(0) = 0 and F(1) =
 *          1.
 * https://oeis.org/A000045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, nice, easy, hear, changed
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 0, y = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x;
}
