/*
 * A163761  a(n) = 10*n*(n+1).
 * https://oeis.org/A163761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n*(n+1)
 * Range:     u8 n <= 4, u16 n <= 80, u32 n <= 875, u64 n <= 875,
 *            u128 n <= 875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * ((seq_t)n + 1) * 10;
}
