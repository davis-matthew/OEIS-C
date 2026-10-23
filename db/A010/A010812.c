/*
 * A010812  24th powers: a(n) = n^24.
 * https://oeis.org/A010812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^24
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t base = (seq_t)n;
    seq_t res = 1;
    for (idx_t i = 0; i < 24; ++i)
        res *= base;
    return res;
}
