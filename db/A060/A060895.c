/*
 * A060895  a(n) = n^16 + 1.
 * https://oeis.org/A060895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^16 + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 255,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t i;
    for (i = 0; i < 16; ++i)
    {
        result *= (seq_t)n;
    }
    return result + 1;
}
