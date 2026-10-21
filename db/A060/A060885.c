/*
 * A060885  a(n) = Sum_{j=0..10} n^j.
 * https://oeis.org/A060885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..10} n^j
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t sn = (seq_t)n;
    for (idx_t i = 0; i < 10; ++i)
        res = res * sn + 1;
    return res;
}
