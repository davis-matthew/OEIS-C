/*
 * A158537  a(n) = 22*n^2 + 1.
 * https://oeis.org/A158537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t r = ((seq_t)22 * x * x) + 1;
    return r;
}
