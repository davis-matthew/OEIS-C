/*
 * A016824  a(n) = (4*n + 1)^12.
 * https://oeis.org/A016824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 1)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 9, u128 n <= 406,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    base = base * 4 + 1;
    seq_t res = 1;
    for (idx_t i = 0; i < 12; ++i)
        res *= base;
    return res;
}
