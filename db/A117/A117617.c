/*
 * A117617  a(n) = 5*a(n-1) + 3 with a(0) = 1.
 * https://oeis.org/A117617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 3 with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                       /* a(0) */
    for (idx_t i = 1; i <= n; ++i)       /* compute up to a(n) */
        val = val * 5 + 3;
    return val;
}
