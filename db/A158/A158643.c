/*
 * A158643  a(n) = 676*n^2 + 26.
 * https://oeis.org/A158643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 676*n^2 + 26
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 2520, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    return ((seq_t)676 * ns * ns) + (seq_t)26;
}
