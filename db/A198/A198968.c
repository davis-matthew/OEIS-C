/*
 * A198968  a(n) = 11*9^n-1.
 * https://oeis.org/A198968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*9^n-1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 10, a(n) = 9*a(n-1) + 8 for n > 0 */
    if (n == 0) return 10;
    seq_t val = 10;
    for (idx_t i = 1; i <= n; ++i)
        val = val * 9 + 8;
    return val;
}
