/*
 * A017592  a(n) = (12*n + 5)^12.
 * https://oeis.org/A017592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 5)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 12 + 5;
    seq_t res  = 1;
    for (int i = 0; i < 12; ++i)
        res *= base;
    return res;
}
