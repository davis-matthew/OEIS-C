/*
 * A067429  Ninth column of triangle A067425.
 * https://oeis.org/A067429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0; a(n) = 12 * 36^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t res = (seq_t)12;
    for (idx_t i = 1; i < n; ++i)
        res *= (seq_t)36;
    return res;
}
