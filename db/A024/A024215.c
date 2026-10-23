/*
 * A024215  Sum of squares of first n positive integers congruent to 1 mod 3.
 * https://oeis.org/A024215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^2 - 3*n - 1)/2
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1126, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t t   = 6 * n_t * n_t - 3 * n_t - 1;   /* 6*n^2 - 3*n - 1 */
    return n_t * t / 2;                        /* n*(6*n^2 - 3*n - 1)/2 */
}
