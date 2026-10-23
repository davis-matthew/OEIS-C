/*
 * A017628  a(n) = (12*n + 8)^12.
 * https://oeis.org/A017628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 8)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * n + (seq_t)8;
    seq_t result = 1;
    for (idx_t i = 0; i < 12; ++i)
        result *= base;
    return result;
}
