/*
 * A158254  a(n) = 289n^2 + 2n.
 * https://oeis.org/A158254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 289*n*n + 2*n
 * Range:     u8 none, u16 n <= 15, u32 n <= 3855, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((seq_t)289 * sn + (seq_t)2);
}
