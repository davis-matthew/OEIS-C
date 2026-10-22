/*
 * A000304  a(n) = a(n-1)*a(n-2).
 * https://oeis.org/A000304
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2), a(2) = 2, a(3) = 3
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 2, y = 3;
    idx_t i;
    if (n <= 2)
        return x;
    for (i = 3; i < n; i++) {
        seq_t z = x * y;
        x = y;
        y = z;
    }
    return y;
}
