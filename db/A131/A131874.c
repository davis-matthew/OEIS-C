/*
 * A131874  a(n) = (7*n^2 + 15*n + 2) / 2.
 * https://oeis.org/A131874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^2 + 15*n + 2) / 2
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t val = 7 * ns * ns + 15 * ns + 2;
    return val / 2;
}
