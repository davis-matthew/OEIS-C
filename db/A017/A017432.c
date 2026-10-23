/*
 * A017432  a(n) = (11*n + 3)^8.
 * https://oeis.org/A017432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)n * (seq_t)11 + (seq_t)3;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 8; ++i)
        result *= base;
    return result;
}
