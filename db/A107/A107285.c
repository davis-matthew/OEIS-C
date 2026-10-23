/*
 * A107285  a(n) = 5*401*(10^n + 1).
 * https://oeis.org/A107285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2005 * (10^n + 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 4010;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        cur = (seq_t)10 * cur - (seq_t)18045;
    return cur;
}
