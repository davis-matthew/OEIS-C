/*
 * A054554  a(n) = 4*n^2 - 10*n + 7.
 * https://oeis.org/A054554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^2 - 10*n + 7
 * Range:     u8 n <= 9, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 in that case. */
    if (n == 0) return 0U;
    seq_t sn = (seq_t)n;      /* make sure arithmetic is done in seq_t */
    return 4 * sn * sn - 10 * sn + 7;
}
