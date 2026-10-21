/*
 * A069477  a(n) = 60*n^2 + 180*n + 150.
 * https://oeis.org/A069477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*n^2 + 180*n + 150
 * Range:     u8 none, u16 n <= 31, u32 n <= 8459, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)60 * sn * sn + (seq_t)180 * sn + (seq_t)150;
    return res;
}
