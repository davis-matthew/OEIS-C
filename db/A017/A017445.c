/*
 * A017445  a(n) = (11*n + 4)^9.
 * https://oeis.org/A017445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 12, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n + 4;
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i)
        result *= base;
    return result;
}
