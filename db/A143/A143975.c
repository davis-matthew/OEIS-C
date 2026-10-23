/*
 * A143975  a(n) = floor(n*(n+3)/3).
 * https://oeis.org/A143975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+3)/3)
 * Range:     u8 n <= 26, u16 n <= 441, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * ((seq_t)n + 3) / 3;
}
