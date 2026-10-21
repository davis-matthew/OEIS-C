/*
 * A067411  Third column of triangle A067410 and second column of A067417.
 * https://oeis.org/A067411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n == 0; a(n) = 4 * 6^(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)4;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)6;
    return result;
}
