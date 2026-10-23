/*
 * A010804  16th powers: a(n) = n^16.
 * https://oeis.org/A010804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^16
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 255,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)n;
    for (int i = 0; i < 16; ++i)
        result *= base;
    return result;
}
