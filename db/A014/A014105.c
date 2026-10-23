/*
 * A014105  Second hexagonal numbers: a(n) = n*(2*n + 1).
 * https://oeis.org/A014105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*(2*n + 1)
 * Range:     u8 n <= 11, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * ((seq_t)2 * x + 1);
}
