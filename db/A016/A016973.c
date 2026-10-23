/*
 * A016973  a(n) = (6*n + 5)^5.
 * https://oeis.org/A016973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 5)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 13, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)5;
    seq_t res  = base;
    res *= base; /* 5th power requires 4 more multiplications */
    res *= base;
    res *= base;
    res *= base;
    return res;
}
