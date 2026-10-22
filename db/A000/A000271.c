/*
 * A000271  Sums of ménage numbers.
 * https://oeis.org/A000271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*a(n-1) + (n-1)*a(n-2) + a(n-3), a(0) = 1, a(1) = 0,
 *            a(2) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1; /* a(i-3) */
    seq_t y = 0; /* a(i-2) */
    seq_t z = 0; /* a(i-1) */
    idx_t i;

    if (n == 0) return x;
    if (n == 1) return y;
    if (n == 2) return z;

    for (i = 3; i <= n; i++) {
        seq_t m = (seq_t)(i - 1);
        seq_t t = m * z + m * y + x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
