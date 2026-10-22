/*
 * A002452  a(n) = (9^n - 1)/8.
 * https://oeis.org/A002452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 9 + 1;
    return r;
}
