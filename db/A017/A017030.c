/*
 * A017030  a(n) = (7*n + 4)^2.
 * https://oeis.org/A017030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^2
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 9361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 7 + 4;
    return x * x;
}
