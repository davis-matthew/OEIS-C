/*
 * A017008  a(n) = (7*n + 2)^4.
 * https://oeis.org/A017008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 36, u64 n <= 9361,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)7 * n + 2;
    seq_t x2 = x * x;
    return x2 * x2;
}
