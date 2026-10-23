/*
 * A157506  a(n) = 13122*n^2 + 324*n + 1.
 * https://oeis.org/A157506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13122*n^2 + 324*n + 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 572, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)13122 * sn * sn + (seq_t)324 * sn + 1;
    return res;
}
