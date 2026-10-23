/*
 * A017564  a(n) = (12*n + 3)^8.
 * https://oeis.org/A017564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 21, u128 n <= 5461,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)12 * (seq_t)n + (seq_t)3;
    seq_t result = base;
    for (int i = 1; i < 8; ++i)
        result *= base;
    return result;
}
