/*
 * A017355  a(n) = (10*n + 7)^3.
 * https://oeis.org/A017355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^3
 * Range:     u8 none, u16 n <= 3, u32 n <= 161, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)10 * n + 7;
    seq_t y = x * x;
    return y * x;
}
