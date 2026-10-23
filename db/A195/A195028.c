/*
 * A195028  a(n) = n*(14*n + 13).
 * https://oeis.org/A195028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(14*n + 13)
 * Range:     u8 n <= 3, u16 n <= 67, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((sn * (seq_t)14) + (seq_t)13);
}
