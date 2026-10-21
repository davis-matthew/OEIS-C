/*
 * A202804  a(n) = n*(6*n+4).
 * https://oeis.org/A202804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n+4)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * ((seq_t)6 * x + (seq_t)4);
}
