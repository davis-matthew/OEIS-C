/*
 * A016917  a(n) = (6*n)^9.
 * https://oeis.org/A016917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 23, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)6;
    seq_t res  = base;
    for (idx_t i = 1; i < 9; ++i)
        res *= base;
    return res;
}
