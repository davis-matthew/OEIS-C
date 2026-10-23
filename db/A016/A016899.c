/*
 * A016899  a(n) = (5n + 4)^3.
 * https://oeis.org/A016899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 324, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)5 * (seq_t)n) + 4;
    return m * m * m;
}
