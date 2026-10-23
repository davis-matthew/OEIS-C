/*
 * A157770  a(n) = 1482401250n^2 - 2134548900n + 768398401.
 * https://oeis.org/A157770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1482401250*n^2 - 2134548900*n + 768398401
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* sequence is defined for n ≥ 1 */

    seq_t sn = (seq_t)n;                 /* convert index to seq_t first */

    seq_t val = (seq_t)1482401250 * sn * sn;
    val -= (seq_t)2134548900 * sn;
    val += (seq_t)768398401;

    return val;
}
