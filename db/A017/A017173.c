/*
 * A017173  a(n) = 9*n + 1.
 * https://oeis.org/A017173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n + 1
 * Range:     u8 n <= 28, u16 n <= 7281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)9 + (seq_t)1;
}
