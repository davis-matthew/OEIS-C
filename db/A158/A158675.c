/*
 * A158675  a(n) = 961*n^2 + 31.
 * https://oeis.org/A158675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 961*n^2 + 31
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 2114, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;
    return sq * (seq_t)961 + (seq_t)31;
}
