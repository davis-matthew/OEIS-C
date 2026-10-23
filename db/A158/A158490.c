/*
 * A158490  a(n) = 100*n^2 - 10.
 * https://oeis.org/A158490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 - 10
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 6553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t val = n_sq * (seq_t)100;
    val -= (seq_t)10;
    return val;
}
