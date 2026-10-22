/*
 * A000213  Tribonacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) with
 *          a(0)=a(1)=a(2)=1.
 * https://oeis.org/A000213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 74, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1, y = 1, z = 1; /* a(i), a(i+1), a(i+2) */
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = x + y + z;
        x = y;
        y = z;
        z = t;
    }
    return x;
}
