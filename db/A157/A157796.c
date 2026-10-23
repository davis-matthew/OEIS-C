/*
 * A157796  a(n) = 27225*n^2 - 12098*n + 1344.
 * https://oeis.org/A157796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27225*n^2 - 12098*n + 1344
 * Range:     u8 none, u16 n <= 1, u32 n <= 397, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    return 27225 * sn * sn - 12098 * sn + 1344;
}
