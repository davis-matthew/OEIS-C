/*
 * A158672  a(n) = 900*n^2 + 30.
 * https://oeis.org/A158672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 900*n^2 + 30
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 2184, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    return ((seq_t)900 * ns * ns) + 30;
}
