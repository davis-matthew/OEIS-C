/*
 * A017187  a(n) = (9*n + 2)^3.
 * https://oeis.org/A017187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 9 + 2;
    return x * x * x;
}
