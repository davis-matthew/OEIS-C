/*
 * A122968  27th powers: a(n) = n^27.
 * https://oeis.org/A122968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n^27
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = (seq_t)n;
    for (idx_t i = 0; i < 27; ++i)
        result *= base;
    return result;
}
