/*
 * A047478  Numbers that are congruent to {1, 5, 7} mod 8.
 * https://oeis.org/A047478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n + floor((n+1)/3)) - 1
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t t = n_seq + (n_seq + 1) / 3;
    return 2 * t - 1;
}
