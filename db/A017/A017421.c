/*
 * A017421  a(n) = (11*n + 2)^9.
 * https://oeis.org/A017421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 12, u128 n <= 1737,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t = x * 11 + 2;
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i)
        result *= t;
    return result;
}
