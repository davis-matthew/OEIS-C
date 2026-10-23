/*
 * A193393  Wiener index of a benzenoid consisting of a zig-zag chain of n
 *          hexagons (s=13; see the Gutman et al. reference).
 * https://oeis.org/A193393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n^3 + 24*n^2 + 62*n - 21)/3
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 928, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;
    seq_t sn3 = sn2 * sn;
    seq_t numerator = sn3 * 16 + sn2 * 24 + sn * 62 - 21;
    return numerator / 3;
}
