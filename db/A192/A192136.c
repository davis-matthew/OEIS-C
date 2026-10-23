/*
 * A192136  a(n) = (5*n^2 - 3*n + 2)/2.
 * https://oeis.org/A192136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^2 - 3*n + 2)/2
 * Range:     u8 n <= 7, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute numerator as (5*n*n - 3*n + 2) */
    seq_t num = (seq_t)5 * n * n - (seq_t)3 * n + 2;
    return num / 2;
}
