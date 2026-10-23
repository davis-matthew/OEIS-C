/*
 * A017289  a(n) = (10*n + 1)^9.
 * https://oeis.org/A017289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 13, u128 n <= 1911,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 10 + 1;
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i)
        result *= base;
    return result;
}
