/*
 * A226490  a(n) = n*(19*n-15)/2.
 * https://oeis.org/A226490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(19*n-15)/2
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = (seq_t)19 * sn - (seq_t)15;
    return (sn * term) / 2;
}
