/*
 * A047345  Numbers that are congruent to {0, 4} mod 7.
 * https://oeis.org/A047345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(7*(n-1)/2)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return ((sn - 1) * 7 + 1) / 2;
}
