/*
 * A199023  a(n) = (6*11^n - 1)/5.
 * https://oeis.org/A199023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 11*a(n-1) + 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        val = ((seq_t)11) * val + (seq_t)2;
    return val;
}
