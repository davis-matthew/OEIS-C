/*
 * A201243  Number of ways to place 2 non-attacking ferses on an n X n board.
 * https://oeis.org/A201243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/2*(n-1)*(n^3 + n^2 - 4n + 4)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 261, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = sn * sn * sn + sn * sn - 4 * sn + 4;
    return ((sn - 1) * term) / 2;
}
