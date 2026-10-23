/*
 * A181617  Molecular topological indices of the complete graph K_n.
 * https://oeis.org/A181617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * n * (n-1)^2
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t sn = (seq_t)n;
    seq_t m  = sn - 1;
    seq_t sq = m * m;
    seq_t res = sn * sq * (seq_t)2;
    return res;
}
