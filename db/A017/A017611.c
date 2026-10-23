/*
 * A017611  a(n) = (12*n + 7)^7.
 * https://oeis.org/A017611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 7)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 46, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    base = base * (seq_t)12 + (seq_t)7;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 7; ++i) result *= base;
    return result;
}
