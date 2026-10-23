/*
 * A017102  a(n) = (8n + 3)^2.
 * https://oeis.org/A017102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8n + 3)^2
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 8 + 3;
    return x * x;
}
