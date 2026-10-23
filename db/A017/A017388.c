/*
 * A017388  a(n) = (10*n + 9)^12.
 * https://oeis.org/A017388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n+9)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 10 + 9;
    seq_t res = base;
    for (int i = 1; i < 12; ++i)
        res *= base;
    return res;
}
