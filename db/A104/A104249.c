/*
 * A104249  a(n) = (3*n^2 + n + 2)/2.
 * https://oeis.org/A104249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + n + 2)/2
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    return (k * k * 3 + k + 2) / 2;
}
