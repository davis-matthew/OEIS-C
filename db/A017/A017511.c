/*
 * A017511  a(n) = (11*n + 10)^3.
 * https://oeis.org/A017511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^3
 * Range:     u8 none, u16 n <= 2, u32 n <= 146, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * n + 10;
    return base * base * base;
}
