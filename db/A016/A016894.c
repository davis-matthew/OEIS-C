/*
 * A016894  a(n) = (5*n + 3)^10.
 * https://oeis.org/A016894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 3)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 16, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5 * n + (seq_t)3;
    seq_t res  = 1;
    for (idx_t i = 0; i < 10; ++i)
        res *= base;
    return res;
}
