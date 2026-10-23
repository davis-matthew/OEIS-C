/*
 * A017661  a(n) = (12*n + 11)^9.
 * https://oeis.org/A017661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 1591,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 12 + 11;
    seq_t res  = 1;
    for (idx_t i = 0; i < 9; ++i)
    {
        res *= base;
    }
    return res;
}
