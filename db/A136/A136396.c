/*
 * A136396  a(n) = 1 + n*(n+1)*(n^2-n+12)/12.
 * https://oeis.org/A136396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n+1)*(n^2-n+12)/12
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t term = n_t * (n_t + 1) *
                 (n_t * n_t - n_t + (seq_t)12);
    term /= (seq_t)12;
    return term + (seq_t)1;
}
