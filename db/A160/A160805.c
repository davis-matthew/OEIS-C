/*
 * A160805  a(n) = (2*n^3 + 9*n^2 + n + 24) / 6.
 * https://oeis.org/A160805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 9*n^2 + n + 24) / 6
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t  = (seq_t)n;
    seq_t n_sq = n_t * n_t;
    seq_t n_cu = n_sq * n_t;
    seq_t term = 2 * n_cu + 9 * n_sq + n_t + 24;
    return term / 6;
}
