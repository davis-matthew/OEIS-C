/*
 * A184636  a(n) = floor(1/{(n^4+2*n)^(1/4)}), where {}=fractional part.
 * https://oeis.org/A184636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 for n = 1, a(n) = 2*n^2 for n >= 2
 * Range:     u8 n <= 11, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 3;
    seq_t m = (seq_t)n;
    return m * m * 2;
}
