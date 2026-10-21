/*
 * A067727  a(n) = 7*n^2 + 14*n.
 * https://oeis.org/A067727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 + 14*n = 7*n*(n+2)
 * Range:     u8 n <= 5, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to seq_t to avoid intermediate overflow */
    seq_t sn = (seq_t)n;
    return sn * (sn + (seq_t)2) * (seq_t)7;
}
