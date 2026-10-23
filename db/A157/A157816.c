/*
 * A157816  a(n) = 1482401250*n^2 - 108900*n + 1.
 * https://oeis.org/A157816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1482401250*n^2 - 108900*n + 1
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t res = 1482401250ULL * tn * tn;
    res -= 108900ULL * tn;
    res += 1;
    return res;
}
