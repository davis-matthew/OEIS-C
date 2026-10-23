/*
 * A158309  a(n) = 361*n^2 + 2*n.
 * https://oeis.org/A158309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 361*n^2 + 2*n
 * Range:     u8 none, u16 n <= 13, u32 n <= 3449, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((seq_t)361 * sn + (seq_t)2);
}
