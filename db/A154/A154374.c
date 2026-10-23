/*
 * A154374  a(n) = 1250*n^2 - 100*n + 1.
 * https://oeis.org/A154374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1250*n^2 - 100*n + 1
 * Range:     u8 none, u16 n <= 7, u32 n <= 1853, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;
    seq_t res = 1250 * sq;
    res -= 100 * sn;
    res += 1;
    return res;
}
