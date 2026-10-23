/*
 * A195317  Centered 40-gonal numbers.
 * https://oeis.org/A195317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^2 - 20*n + 1
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return 20 * sn * sn - 20 * sn + 1;
}
